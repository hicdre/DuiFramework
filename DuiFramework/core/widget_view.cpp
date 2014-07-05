#include "stdafx.h"
#include "widget_view.h"

#include "core/event.h"

namespace ui
{

	Widget* WidgetView::CreateWidget(WidgetDelegate* delegate)
	{
		WidgetView* view = new WidgetView(delegate);
		view->Init();
		return const_cast<Widget*>(view->GetWidget());
		//ui::Widget* widget = ui::Widget::Create(NULL, delegate->GetInitialRect());
		//widget->SetView(new WidgetView(delegate));
	}

	WidgetView::WidgetView(WidgetDelegate* delegate)
		: delegate_(delegate)
	{

	}

	WidgetView::~WidgetView()
	{

	}

	void WidgetView::Init()
	{
		SetCursor(::LoadCursor(NULL, IDC_ARROW));

		owned_widget_ = ui::Widget::Create(NULL, GetInitialRect());
		owned_widget_->SetMessageHanler(this);
		Rect view_rect = owned_widget_->GetWindowScreenBounds();
		SetBounds(0, 0, view_rect.width(), view_rect.height());

		delegate_->Init(this);
	}

	Rect WidgetView::GetInitialRect()
	{
		return delegate_->GetInitialRect();
	}

	const Widget* WidgetView::GetWidget() const
	{
		return owned_widget_;
	}

	BOOL WidgetView::ProcessWindowMessage(HWND window, UINT message, WPARAM w_param, LPARAM l_param, LRESULT& result)
	{
		//消息分发
		if (message == WM_PAINT)
		{
			Painter painter(owned_widget_);
			DoPaint(&painter);
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

	void WidgetView::SchedulePaintInRect(const Rect& r)
	{
		if (!visible_ || !painting_enabled_)
			return;

		Rect invalid_rect(GetLocalBounds());
		invalid_rect.Intersect(r);
		if (!invalid_rect.IsEmpty())
			widget()->InvalidateRect(invalid_rect);
	}

	LRESULT WidgetView::HandleMouseEvent(UINT message, WPARAM w_param, LPARAM l_param)
	{
		msg_ = { widget()->hwnd(), message, w_param, l_param, GetMessageTime(),
			{ ((int)(short)LOWORD(l_param)), ((int)(short)HIWORD(l_param)) } };

		Point pt = GetMouseLocation(msg_);
		if (mouse_position_ != pt)
		{
			UpdateMousePosition(pt);
		}

		long message_time = GetMessageTime();
		

		return 0;
	}

	void WidgetView::UpdateMousePosition(const Point& new_pos)
	{
		Point old_pos = mouse_position_;
		mouse_position_ = new_pos;
		View* old_view = hitttest_view_;
		hitttest_view_ = Hittest(new_pos);

		if (hitttest_view_) {
			//设置鼠标指针
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
		//DispatchMouseMoveEvent(new_pos);
	}

	void WidgetView::DispatchMouseLeaveEvent(View* from, View* to)
	{
		MouseEvent event(EVENT_MOUSE_LEAVE,
			from->ConvertPointFromWidget(mouse_position_),
			from,
			GetEventFlags(msg_));

		if (to == NULL)
			to = this;

		for (View* v = from; v != to;)
		{
			event.DispathTo(v);
			v->HandleEvent(&event);
			v = v->parent();
		}

		event.DispathTo(to);
		to->HandleEvent(&event);
	}

	void WidgetView::DispatchMouseEnterEvent(View* from, View* to)
	{
		Views views;
		if (from == NULL)
			from = this;
		for (View* v = to; v != from; v = v->parent())
			views.push_back(v);
		views.push_back(from);


		auto iter = views.rbegin();
		View* v = *iter;
		MouseEvent event(EVENT_MOUSE_ENTER,
			v->ConvertPointFromWidget(mouse_position_),
			v,
			GetEventFlags(msg_));
		v->HandleEvent(&event);
		iter++;
		while (iter != views.rend())
		{
			v = *iter;
			event.DispathTo(v);
			v->HandleEvent(&event);
			iter++;
		}
		
	}

	void WidgetView::DispatchMouseMoveEvent(View* from)
	{
		
		MouseEvent event(EVENT_MOUSE_MOVE, 
			from->ConvertPointFromWidget(mouse_position_), 
			from,
			GetEventFlags(msg_));
		for (View* v = from; v != NULL; )
		{
			v->HandleEvent(&event);
			v = v->parent();
			event.DispathTo(v);
		}
	}

	void WidgetView::CenterWidget()
	{
		// Calculate the ideal bounds.
		HWND parent = NULL;
		HWND window = widget()->hwnd();
		Size pref(size());

		RECT center_bounds = { 0 };
			// No parent or no parent rect. Center over the monitor the window is on.
		HMONITOR monitor = MonitorFromWindow(window, MONITOR_DEFAULTTONEAREST);
		if (monitor) 
		{
			MONITORINFO mi = { 0 };
			mi.cbSize = sizeof(mi);
			GetMonitorInfo(monitor, &mi);
			center_bounds = mi.rcWork;
		}
		else 
		{
			assert(0); // << "Unable to get default monitor";
		}

		Rect window_bounds(center_bounds);
		int x = (window_bounds.width() - width()) / 2;
		int y = (window_bounds.height() - height()) / 2;

		widget()->SetWindowPos(x, y);

	}

}