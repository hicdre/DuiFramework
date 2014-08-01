#pragma once
#include "event/event.h"
#include "event/mouse_event.h"
#include "event/key_event.h"

namespace ui
{

	class View;
	class Window;
	class MouseEventHandler
	{
	public:
		MouseEventHandler(Window* window);

		void HandleMouseEvent(UINT message,
			WPARAM w_param,
			LPARAM l_param);

		void DispatchPropagation(Event* evt, View* from);

		void SetFocus(View* v);
		View* GetFocusedView() const;
	private:
		

// 		LRESULT HandleKeyEvent(UINT message,
// 			WPARAM w_param,
// 			LPARAM l_param);
// 
// 		LRESULT HandleFocus(UINT message,
// 			WPARAM w_param,
// 			LPARAM l_param);

		void HandleMouseMove(const Point& pt);

		void DispatchMouseLeaveEvent(View* from, View* to);
		void DispatchMouseEnterEvent(View* from, View* to);
		void DispatchMouseMoveEvent(View* from);
		void DispatchMouseDownEvent(View* from, int buttons);
		void DispatchMouseUpEvent(View* from, int buttons);
		void DispatchMouseDbClickEvent(View* from, int buttons);//click和dbclick事件先暂时放一边

		//void DispatchMousePressEvent(View* from, WPARAM w_param, LPARAM l_param);
		//void DispatchMouseReleaseEvent(View* from, WPARAM w_param, LPARAM l_param);

		View* view() const;
		Widget* widget() const;

		Window* window_;

		Point mouse_position_;
		View* hitttest_view_{ NULL };
	};

}