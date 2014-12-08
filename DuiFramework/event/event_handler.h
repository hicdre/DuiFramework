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

		bool HandleMouseEvent(MouseEvent* evt);
		bool HandleMouseMoveEvent(MouseEvent* evt);
		bool HandleMousePressEvent(MouseEvent* evt);

	private:
		void UpdateElementHoverState(MouseEvent* evt, HitTestResult* result);
		void UpdateMouseEventTarget(UIElement* target, MouseEvent* evt);
		bool DispatchMouseEvent(EventType type, UIElement* target, int clickCount, MouseEvent* evt, bool setUnder);
		UIWindow* window_;

		UIElement* element_under_mouse_{NULL};
		UIElement* last_element_under_mouse_{NULL};

		scoped_refptr<MouseEvent> last_mouse_event_;
	};
}