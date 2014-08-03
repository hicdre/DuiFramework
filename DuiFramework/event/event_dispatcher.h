#pragma once
#include "event/event.h"

namespace ui
{
	class Window;
	typedef std::unordered_map < View*,
		std::unordered_map < EventType, EventHandlerList* >> EventDispatchMap;
	class EventDispatcher
	{
	public:
		static EventDispatcher* FindByWindow(Window* window);
		static void ReleaseAll();

		void RemoveByView(View* v);



	private:
		~EventDispatcher();
		EventDispatchMap dispatch_map_;
	};
}