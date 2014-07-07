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
		case WM_MBUTTONDOWN:
		case WM_NCMBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_NCRBUTTONDOWN:
		case WM_XBUTTONDOWN:
		case WM_NCXBUTTONDOWN:
			DispatchMouseDownEvent(hitttest_view_);
			break;
		case WM_LBUTTONUP:
		case WM_NCLBUTTONUP:
		case WM_MBUTTONUP:
		case WM_NCMBUTTONUP:
		case WM_RBUTTONUP:
		case WM_NCRBUTTONUP:
		case WM_XBUTTONUP:
		case WM_NCXBUTTONUP:
			DispatchMouseUpEvent(hitttest_view_);
			break;
		case WM_LBUTTONDBLCLK:
		case WM_NCLBUTTONDBLCLK:
		case WM_MBUTTONDBLCLK:
		case WM_NCMBUTTONDBLCLK:
		case WM_RBUTTONDBLCLK:
		case WM_NCRBUTTONDBLCLK:
		case WM_XBUTTONDBLCLK:
		case WM_NCXBUTTONDBLCLK:
			DispatchMouseDbClickEvent(hitttest_view_);
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

	MouseEvent* EventDispatcher::CreateMouseEvent(EventType type, View* owner)
	{
		return new MouseEvent(type, mouse_position_, owner);
	}

	void EventDispatcher::DispatchMouseMoveEvent(View* from)
	{
		scoped_ptr<MouseEvent> evt(CreateMouseEvent(EVENT_MOUSE_MOVE, from));
		for (View* v = from; v != NULL; v = v->parent())
		{
			if (!evt->stopped_propagation())
				MouseEvent::DispatchTo(evt.get(), v);
		}
	}

	void EventDispatcher::DispatchMouseLeaveEvent(View* from, View* to)
	{
		if (to == NULL)
			to = view();
		scoped_ptr<MouseEvent> evt(CreateMouseEvent(EVENT_MOUSE_LEAVE, from));
		for (View* v = from; v != to; v = v->parent())
		{
			if (!evt->stopped_propagation())
				MouseEvent::DispatchTo(evt.get(), v);
		}
		if (!evt->stopped_propagation())
			MouseEvent::DispatchTo(evt.get(), to);
	}

	void EventDispatcher::DispatchMouseEnterEvent(View* from, View* to)
	{
		Views views;
		if (from == NULL)
			from = view();
		for (View* v = to; v != from; v = v->parent())
			views.push_back(v);
		views.push_back(from);

		scoped_ptr<MouseEvent> evt(CreateMouseEvent(EVENT_MOUSE_ENTER, to));
		for (auto iter = views.rbegin(); iter != views.rend(); iter++)
		{
			MouseEvent::DispatchTo(evt.get(), *iter);
		}
	}

	void EventDispatcher::DispatchMouseDownEvent(View* from)
	{
		scoped_ptr<MouseEvent> evt(CreateMouseEvent(EVENT_MOUSE_DOWN, from));
		for (View* v = from; v != NULL; v = v->parent())
		{
			if (!evt->stopped_propagation())
				MouseEvent::DispatchTo(evt.get(), v);
		}
	}

	void EventDispatcher::DispatchMouseUpEvent(View* from)
	{
		scoped_ptr<MouseEvent> evt(CreateMouseEvent(EVENT_MOUSE_UP, from));
		for (View* v = from; v != NULL; v = v->parent())
		{
			if (!evt->stopped_propagation())
				MouseEvent::DispatchTo(evt.get(), v);
		}
	}

	void EventDispatcher::DispatchMouseDbClickEvent(View* from)
	{
		scoped_ptr<MouseEvent> evt(CreateMouseEvent(EVENT_MOUSE_DBCLICK, from));
		for (View* v = from; v != NULL; v = v->parent())
		{
			if (!evt->stopped_propagation())
				MouseEvent::DispatchTo(evt.get(), v);
		}
	}

	

}