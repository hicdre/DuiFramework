#pragma once
#include "event/event.h"
#include "event/mouse_event.h"

namespace ui
{
	class WidgetView;
	class View;
	class EventDispatcher
	{
	public:
		EventDispatcher(WidgetView* view);

		BOOL ProcessWindowMessage(HWND window, UINT message, WPARAM w_param, LPARAM l_param, LRESULT& result);

		void DispatchPropagation(Event* evt, View* from);
	private:
		LRESULT HandleMouseEvent(UINT message,
			WPARAM w_param,
			LPARAM l_param);

		LRESULT HandleKeyEvent(UINT message,
			WPARAM w_param,
			LPARAM l_param);

		void HandleMouseMove(const Point& pt);

		void DispatchMouseLeaveEvent(View* from, View* to);
		void DispatchMouseEnterEvent(View* from, View* to);
		void DispatchMouseMoveEvent(View* from);
		void DispatchMouseDownEvent(View* from, int buttons);
		void DispatchMouseUpEvent(View* from, int buttons);
		void DispatchMouseDbClickEvent(View* from, int buttons);//click和dbclick事件先暂时放一边

		WidgetView* view() const { return view_; }
		Widget* widget() const;
		WidgetView* view_;

		Point mouse_position_;
		View* hitttest_view_{ NULL };
	};
}