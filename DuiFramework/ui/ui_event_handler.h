#pragma once
#include "event/event_forward.h"
#include "ui/ui_forward.h"


namespace ui
{
	class HitTestResult;
	class UIEventHandler
	{
	public:
		explicit UIEventHandler(UIWindow* window);
		~UIEventHandler();

		bool HandleMouseEvent(MouseEvent* evt);
		bool HandleMouseMoveEvent(MouseEvent* evt);
		bool HandleMousePressEvent(MouseEvent* evt);
		bool HandleMouseReleaseEvent(MouseEvent* evt);

	private:
		void UpdateElementHoverState(MouseEvent* evt, HitTestResult* result);
		void UpdateElementActiveFocusState();
		void UpdateMouseEventTarget(UIElement* target, MouseEvent* evt);
		void UpdateCursor();
		bool DispatchMouseEvent(EventType type, UIElement* target, int clickCount, MouseEvent* evt, bool setUnder);
		UIWindow* window_;

		UIElement* element_under_mouse_{NULL};
		UIElement* last_element_under_mouse_{NULL};

		UIElement* element_pressed_{ NULL };
		UIElement* element_focused_{ NULL };

		bool may_start_drag_{ false };

		scoped_refptr<MouseEvent> last_mouse_event_;
		CursorId last_cursor{ Cursor_Inherit };
	};
}