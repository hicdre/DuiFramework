#pragma once
#include "event/event_forward.h"
#include "event/event_listener.h"

namespace ui
{
	class EventTarget : public RefCounted<EventTarget>
	{
	public:
		EventTarget();
		virtual ~EventTarget();

		virtual bool AddEventListener(EventType eventType, EventListener* listener);
		virtual bool RemoveEventListener(EventType eventType, EventListener* listener);
		virtual void RemoveAllEventListeners();
		virtual bool DispatchEvent(scoped_refptr<Event> event);

		bool HasEventListeners() const;
		bool HasEventListeners(EventType eventType) const;
		std::vector<EventType> eventTypes();

		bool FireEventListeners(Event* event);
	private:
		void FireEventListeners(Event* event, EventListenerVector* vec);

		EventListenerMap event_listener_map_;
	};
}