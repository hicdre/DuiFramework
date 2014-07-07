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
	private:
		LRESULT HandleMouseEvent(UINT message,
			WPARAM w_param,
			LPARAM l_param);

		void HandleMouseMove(const Point& pt);

		MouseEvent* CreateMouseEvent(EventType type, View* owner);

		void DispatchMouseLeaveEvent(View* from, View* to);
		void DispatchMouseEnterEvent(View* from, View* to);
		void DispatchMouseMoveEvent(View* from);
		void DispatchMouseDownEvent(View* from);
		void DispatchMouseUpEvent(View* from);
		void DispatchMouseDbClickEvent(View* from);//click和dbclick事件先暂时放一边

		WidgetView* view() const { return view_; }
		Widget* widget() const;
		WidgetView* view_;

		Point mouse_position_;
		View* hitttest_view_{ NULL };
	};
}