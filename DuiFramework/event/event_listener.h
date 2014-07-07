#pragma once
#include "event/event.h"

namespace ui
{
	class EventListener
	{
	public:
		EventListener();
		virtual ~EventListener();

		EventListener& Listen(View* from, EventType type, EventAction action);
		void UnListen(View* from, EventType type);
		void UnListen(View* from);

		void DispatchEvent(View* v, Event* e);
	private:
		EventListenMap event_map_;
	};
}