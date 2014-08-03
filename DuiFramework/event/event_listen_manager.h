#pragma once
#include <vector>
#include "event/event_listener.h"

namespace ui
{
	class EventListenManager
	{
	public:
		EventListenManager();
		~EventListenManager();

		void AddListener(EventListener* listener);
		void RemoveListener(EventListener* listener);

		void RemoveView(View* v);

		void InvokeEvent(View* v, Event* e);

		static EventListenManager* Default();
	private:
		std::vector<EventListener*> listener_list_;
	};
}