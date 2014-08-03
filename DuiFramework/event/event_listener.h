#pragma once
#include "event/event.h"

namespace ui
{
	


	class EventListener
	{
	public:
		EventListener();
		virtual ~EventListener();

		EventListener& Listen(View* from, EventType type, EventMethod action);
		void UnListen(View* from, EventType type);
		void UnListen(View* from);

		void DispatchEvent(View* v, Event* e);
	private:
		EventHandlerList event_handler_list_;
	};
}