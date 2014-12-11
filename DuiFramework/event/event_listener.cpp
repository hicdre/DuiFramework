#include "stdafx.h"
#include "event_listener.h"
#include "event_include.h"

namespace ui
{


	EventListenerMap::EventListenerMap()
	{

	}


	EventListenerMap::~EventListenerMap()
	{
		clear();
	}


	bool EventListenerMap::contains(EventType eventType) const
	{
		return entries_.count(eventType) > 0;
	}

	void EventListenerMap::clear()
	{
		entries_.clear();
	}

	static bool addListenerToVector(EventListenerVector* vector, EventListener* listener)
	{
		RegisteredEventListener registeredListener(listener);

		if (std::find(vector->begin(), vector->end(), registeredListener) != vector->end())
			return false; // Duplicate listener.

		vector->push_back(registeredListener);
		return true;
	}

	bool EventListenerMap::add(EventType eventType, EventListener* listener)
	{
		auto iter = entries_.find(eventType);
		if (iter != entries_.end()) {
			return addListenerToVector(&(iter->second), listener);
		}

		EventListenerVector vec;
		if (addListenerToVector(&vec, listener)) {
			entries_[eventType] = std::move(vec);
			return true;
		}
		return false;
	}

	static bool removeListenerFromVector(EventListenerVector* vector, EventListener* listener, size_t& indexOfRemovedListener)
	{
		RegisteredEventListener registeredListener(listener);
		indexOfRemovedListener = 0;
		for (auto iter = vector->begin(); iter != vector->end(); ++iter, ++indexOfRemovedListener)
		{
			if (*iter == registeredListener) {
				vector->erase(iter);
				return true;
			}
		}
		return false;
	}

	bool EventListenerMap::remove(EventType eventType, EventListener* listener, size_t& indexOfRemovedListener)
	{
		auto iter = entries_.find(eventType);
		if (iter == entries_.end()) {
			return false;
		}
		bool wasRemoved = removeListenerFromVector(&(iter->second), listener, indexOfRemovedListener);
		if (iter->second.empty())
			entries_.erase(iter);
		return wasRemoved;
	}

	EventListenerVector* EventListenerMap::find(EventType eventType)
	{
		auto iter = entries_.find(eventType);
		if (iter == entries_.end()) {
			return NULL;
		}
		return &(iter->second);
	}

	std::vector<EventType> EventListenerMap::eventTypes() const
	{
		std::vector<EventType> ret;
		for (auto iter : entries_)
		{
			ret.push_back(iter.first);
		}
		return ret;
	}

}