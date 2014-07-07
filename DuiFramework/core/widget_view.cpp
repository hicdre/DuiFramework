#include "stdafx.h"
#include "widget_view.h"

#include "event/event_dispatcher.h"

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
		: event_delegate_(delegate)
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

		dispatcher_.reset(new EventDispatcher(this));

		event_delegate_->Init(this);
	}

	Rect WidgetView::GetInitialRect()
	{
		return event_delegate_->GetInitialRect();
	}

	const Widget* WidgetView::GetWidget() const
	{
		return owned_widget_;
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


	BOOL WidgetView::ProcessWindowMessage(HWND window, UINT message, WPARAM w_param, LPARAM l_param, LRESULT& result)
	{
		if (dispatcher_.get())
			return dispatcher_->ProcessWindowMessage(window, message, w_param, l_param, result);

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


}