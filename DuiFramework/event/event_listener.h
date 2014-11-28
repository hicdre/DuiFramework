#pragma once
#include "event/event_forward.h"
#include <vector>
#include <unordered_map>

namespace ui
{
	class EventListener : public RefCounted<EventListener> {
	public:
		enum Type {
			//JSEventListenerType,
			ImageEventListenerType,
			CPPEventListenerType,
			ConditionEventListenerType,
			NativeEventListenerType,
		};

		virtual ~EventListener() { }
		virtual void HandleEvent(Event*) = 0;

		Type type() const { return type_; }

	protected:
		explicit EventListener(Type type) : type_(type)	{
		}

	private:
		Type type_;
	};

	typedef scoped_refptr<EventListener> RegisteredEventListener;
	typedef std::vector<RegisteredEventListener> EventListenerVector;

	class EventListenerMap {
	public:
		EventListenerMap();
		~EventListenerMap();

		bool isEmpty() const { return entries_.empty(); }
		bool contains(EventType eventType) const;
		//bool containsCapturing(const AtomicString& eventType) const;

		void clear();
		bool add(EventType eventType, EventListener*);
		bool remove(EventType eventType, EventListener*, size_t& indexOfRemovedListener);
		EventListenerVector* find(EventType eventType);
		std::vector<EventType> eventTypes() const;

	private:

		std::unordered_map<EventType, EventListenerVector> entries_;
	};
}