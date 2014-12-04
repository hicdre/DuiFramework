#include "stdafx.h"
#include "ui_window.h"
#include "ui_include.h"
#include "render/render_include.h"
#include "event/input_event_builder.h"

namespace ui
{


	UIWindow::UIWindow(UIDocumentPtr doc)
		: UIElement(doc)
		, event_handler_(this)
	{
		
	}

	UIWindow::~UIWindow()
	{
		owned_widget_->SetMessageHanler(NULL);
		delete owned_widget_;
	}

	void UIWindow::SchedulePaintInRect(const Rect& r)
	{
		if (!r.IsEmpty())
			owned_widget_->InvalidateRect(r);
	}

	BOOL UIWindow::ProcessWindowMessage(HWND window, UINT message, WPARAM w_param, LPARAM l_param, LRESULT& result)
	{
		if (message == WM_PAINT)
		{
			RenderContext painter(owned_widget_);
			DoPaint(&painter, GetLocalBounds());
			return TRUE;
		}
		else if (message == WM_SIZE)
		{
			Size sz = { LOWORD(l_param), HIWORD(l_param) };
			SetSize(sz);
			return TRUE;
		}
		else if ((message >= WM_MOUSEFIRST && message <= WM_MOUSELAST)
			|| (message >= WM_NCMOUSEMOVE && message <= WM_NCXBUTTONDBLCLK)
			|| message == WM_MOUSELEAVE
			|| message == WM_NCMOUSELEAVE)
		{
			scoped_refptr<MouseEvent> event = BuildMouseEvent(owned_widget_->hwnd(), message, w_param, l_param, GetTickCount());
			event_handler_.HandleMouseMoveEvent(event.get());
			return TRUE;
		}
		return FALSE;
	}

	void UIWindow::AttatchWidget(Widget* widget)
	{
		owned_widget_ = widget;
		owned_widget_->SetMessageHanler(this);

		owned_widget_->SetSize(Size(styles()->pixelWidth(), styles()->pixelHeight()));

		SetCursor(::LoadCursor(NULL, IDC_ARROW));
	}

	

}