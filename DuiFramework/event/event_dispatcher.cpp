#include "stdafx.h"
#include "event_dispatcher.h"

#include "core/widget.h"
#include "core/widget_view.h"
#include "core/view.h"

namespace ui
{
	EventDispatcher::EventDispatcher(WidgetView* view)
		: view_(view)
	{

	}

	Widget* EventDispatcher::widget() const
	{
		return const_cast<Widget*>(view()->GetWidget());
	}

	BOOL EventDispatcher::ProcessWindowMessage(HWND window, UINT message, WPARAM w_param, LPARAM l_param, LRESULT& result)
	{
		if (message == WM_PAINT)
		{
			Painter painter(widget());
			view()->DoPaint(&painter);
			return TRUE;
		}
		else if ((message >= WM_MOUSEFIRST && message <= WM_MOUSELAST)
			|| (message >= WM_NCMOUSEMOVE && message <= WM_NCXBUTTONDBLCLK)
			|| message == WM_MOUSELEAVE
			|| message == WM_NCMOUSELEAVE)
		{
			result = HandleMouseEvent(message, w_param, l_param);
			return TRUE;
		}
		else if ( message == WM_KEYDOWN
			|| message == WM_KEYUP
			|| message == WM_SYSKEYDOWN
			|| message == WM_SYSKEYUP)
		{
			result = HandleMouseEvent(message, w_param, l_param);
			return TRUE;
		}
		return FALSE;
	}

	LRESULT EventDispatcher::HandleMouseEvent(UINT message, WPARAM w_param, LPARAM l_param)
	{
		Point pt = GetMousePosition(widget());

		HandleMouseMove(pt);

		switch (message)
		{
		case WM_LBUTTONDOWN:
		case WM_NCLBUTTONDOWN:
			DispatchMouseDownEvent(hitttest_view_, MOUSE_LEFT);
			break;
		case WM_MBUTTONDOWN:
		case WM_NCMBUTTONDOWN:
			DispatchMouseDownEvent(hitttest_view_, MOUSE_MIDDLE);
			break;
		case WM_RBUTTONDOWN:
		case WM_NCRBUTTONDOWN:
			DispatchMouseDownEvent(hitttest_view_, MOUSE_RIGHT);
			break;
		case WM_XBUTTONDOWN:
		case WM_NCXBUTTONDOWN:
			//DispatchMouseDownEvent(hitttest_view_, MOUSE_LEFT);
			break;
		case WM_LBUTTONUP:
		case WM_NCLBUTTONUP:
			DispatchMouseUpEvent(hitttest_view_, MOUSE_LEFT);
			break;
		case WM_MBUTTONUP:
		case WM_NCMBUTTONUP:
			DispatchMouseUpEvent(hitttest_view_, MOUSE_MIDDLE);
			break;
		case WM_RBUTTONUP:
		case WM_NCRBUTTONUP:
			DispatchMouseUpEvent(hitttest_view_, MOUSE_RIGHT);
			break;
		case WM_XBUTTONUP:
		case WM_NCXBUTTONUP:
			//DispatchMouseUpEvent(hitttest_view_);
			break;
		case WM_LBUTTONDBLCLK:
		case WM_NCLBUTTONDBLCLK:
			DispatchMouseDbClickEvent(hitttest_view_, MOUSE_LEFT);
			break;
		case WM_MBUTTONDBLCLK:
		case WM_NCMBUTTONDBLCLK:
			DispatchMouseDbClickEvent(hitttest_view_, MOUSE_MIDDLE);
			break;
		case WM_RBUTTONDBLCLK:
		case WM_NCRBUTTONDBLCLK:
			DispatchMouseDbClickEvent(hitttest_view_, MOUSE_RIGHT);
			break;
		case WM_XBUTTONDBLCLK:
		case WM_NCXBUTTONDBLCLK:
			//DispatchMouseDbClickEvent(hitttest_view_);
			break;
		default:
			break;
		}
		return 0;
	}

	void EventDispatcher::HandleMouseMove(const Point& pt)
	{
		if (mouse_position_ == pt)
			return;

		Point old_pos = mouse_position_;
		mouse_position_ = pt;
		View* old_view = hitttest_view_;
		hitttest_view_ = view()->Hittest(pt);

		if (hitttest_view_) {
			//ÉèÖÃÊó±êÖ¸Õë
			HCURSOR cursor = hitttest_view_->GetCursor();
			widget()->SetCursor(cursor);

			DispatchMouseMoveEvent(hitttest_view_);
		}

		if (old_view == hitttest_view_)
			return;

		if (old_view == NULL) {
			DispatchMouseEnterEvent(old_view, hitttest_view_);

			TRACKMOUSEEVENT tme = { 0 };
			tme.cbSize = sizeof(TRACKMOUSEEVENT);
			tme.dwFlags = TME_LEAVE;
			tme.hwndTrack = widget()->hwnd();
			tme.dwHoverTime = HOVER_DEFAULT;
			::TrackMouseEvent(&tme);
			return;
		}

		if (hitttest_view_ == NULL) {
			DispatchMouseLeaveEvent(old_view, hitttest_view_);
			return;
		}

		View* public_ancestor = hitttest_view_->GetAncestorTo(old_view);
		if (public_ancestor == NULL)
			return;
		DispatchMouseLeaveEvent(old_view, public_ancestor);
		DispatchMouseEnterEvent(public_ancestor, hitttest_view_);
	}

	void EventDispatcher::DispatchMouseMoveEvent(View* from)
	{
		MouseEvent evt(EVENT_MOUSE_MOVE, mouse_position_, from);
		DispatchPropagation(&evt, from);
	}

	void EventDispatcher::DispatchMouseLeaveEvent(View* from, View* to)
	{
		if (to == NULL)
			to = view();
		
		for (View* v = from; v != to; v = v->parent())
		{
			MouseEvent evt(EVENT_MOUSE_LEAVE, mouse_position_, v);
			v->HandleEvent(&evt);
		}
		MouseEvent evt(EVENT_MOUSE_LEAVE, mouse_position_, to);
		to->HandleEvent(&evt);
	}

	void EventDispatcher::DispatchMouseEnterEvent(View* from, View* to)
	{
		Views views;
		if (from == NULL)
			from = view();
		for (View* v = to; v != from; v = v->parent())
			views.push_back(v);
		views.push_back(from);

		for (auto iter = views.rbegin(); iter != views.rend(); iter++)
		{
			MouseEvent evt(EVENT_MOUSE_ENTER, mouse_position_, *iter);
			(*iter)->HandleEvent(&evt);
		}
	}

	void EventDispatcher::DispatchMouseDownEvent(View* from, int buttons)
	{
		MouseDownEvent evt(EVENT_MOUSE_DOWN, mouse_position_, from, buttons);
		DispatchPropagation(&evt, from);
	}

	void EventDispatcher::DispatchMouseUpEvent(View* from, int buttons)
	{
		MouseUpEvent evt(EVENT_MOUSE_UP, mouse_position_, from, buttons);
		DispatchPropagation(&evt, from);
	}

	void EventDispatcher::DispatchMouseDbClickEvent(View* from, int buttons)
	{
		MouseUpEvent evt(EVENT_MOUSE_DBCLICK, mouse_position_, from, buttons);
		DispatchPropagation(&evt, from);
	}

	void EventDispatcher::DispatchPropagation(Event* evt, View* from)
	{
		for (View* v = from; v != NULL; v = v->parent())
		{
			if (!evt->stopped_propagation()) {
				v->HandleEvent(evt);
			}
		}
	}

	LRESULT EventDispatcher::HandleKeyEvent(UINT message, WPARAM w_param, LPARAM l_param)
	{

	}

	

}