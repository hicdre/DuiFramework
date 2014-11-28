#include "stdafx.h"
#include "event_target.h"
#include "event_include.h"

namespace ui
{


	EventTarget::EventTarget()
	{

	}

	EventTarget::~EventTarget()
	{

	}

	bool EventTarget::AddEventListener(EventType eventType, EventListener* listener)
	{
		return event_listener_map_.add(eventType, listener);
	}

	bool EventTarget::RemoveEventListener(EventType eventType, EventListener* listener)
	{
		size_t indexOfRemovedListener;
		if (!event_listener_map_.remove(eventType, listener, indexOfRemovedListener))
			return false;

		return true;
	}

	void EventTarget::RemoveAllEventListeners()
	{
		event_listener_map_.clear();

		// Notify firing events planning to invoke the listener at 'index' that
		// they have one less listener to invoke.
		// 		if (d->firingEventIterators) {
		// 			for (size_t i = 0; i < d->firingEventIterators->size(); ++i) {
		// 				d->firingEventIterators->at(i).iterator = 0;
		// 				d->firingEventIterators->at(i).end = 0;
		// 			}
		// 		}
	}

	bool EventTarget::DispatchEvent(scoped_refptr<Event> event)
	{
		event->setTarget(this);
		event->setCurrentTarget(this);
		event->setEventPhase(EVENT_AT_TARGET);
		bool defaultPrevented = FireEventListeners(event.get());
		event->setEventPhase(EVENT_NONE);
		return defaultPrevented;
	}

	bool EventTarget::HasEventListeners() const
	{
		return !event_listener_map_.isEmpty();
	}

	bool EventTarget::HasEventListeners(EventType eventType) const
	{
		return event_listener_map_.contains(eventType);
	}

	std::vector<EventType> EventTarget::eventTypes()
	{
		return event_listener_map_.eventTypes();
	}

	bool EventTarget::FireEventListeners(Event* event)
	{
		EventListenerVector* listenersVector = event_listener_map_.find(event->type());
		if (listenersVector) {
			FireEventListeners(event, listenersVector);
		}
		return !event->defaultPrevented();
	}

	void EventTarget::FireEventListeners(Event* event, EventListenerVector* vec)
	{
		size_t size = vec->size();
		for (size_t i = 0; i < size; ++i) {
			RegisteredEventListener& registeredListener = vec->at(i);
			if (event->eventPhase() == EVENT_CAPTURING_PHASE) {
				assert(0);
				continue;
			}
				
			if (event->immediatePropagationStopped())
				break;

			registeredListener->HandleEvent(event);
		}
	}

}
