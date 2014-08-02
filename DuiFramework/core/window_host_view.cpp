#include "stdafx.h"
#include "window_host_view.h"

#include "control/window.h"
#include "event/event_handler.h"

namespace ui
{


	WindowHostView::WindowHostView(Window* window)
		: window_(window)
	{
	}

	WindowHostView::~WindowHostView()
	{
	}

	void WindowHostView::SchedulePaintInRect(const Rect& r)
	{
		if (!visible_ || !painting_enabled_)
			return;

		Rect invalid_rect(GetLocalBounds());
		invalid_rect.Intersect(r);
		if (!invalid_rect.IsEmpty())
		{
			window_->SchedulePaint(r);
			//dispatcher_.Invoke(this, r);
		}
			//widget()->InvalidateRect(invalid_rect);
	}

}