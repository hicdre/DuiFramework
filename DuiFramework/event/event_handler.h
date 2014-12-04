#pragma once
#include "event_forward.h"
#include "dom/ui_forward.h"


namespace ui
{
	class HitTestResult;
	class EventHandler
	{
	public:
		explicit EventHandler(UIWindow* window);
		~EventHandler();

		bool HandleMouseMoveEvent(MouseEvent* evt);
		bool HandleMouseMoveOrLeaveEvent(MouseEvent* evt, HitTestResult* result);

	private:
		void UpdateMouseEventTarget(UIElement* target, MouseEvent* evt, bool fireMouseOut);
		bool DispatchMouseEvent(EventType type, UIElement* target, int clickCount, MouseEvent* evt, bool setUnder);
		UIWindow* window_;

		UIElement* element_under_mouse_{NULL};
		UIElement* last_element_under_mouse_{NULL};
	};
}