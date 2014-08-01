#include "stdafx.h"
#include "window.h"

#include "event/event_handler.h"
#include "render/painter.h"
#include "core/view.h"
#include "core/window_host_view.h"

namespace ui
{


	Window::Window()
	{
		Init();
	}

	Window::~Window()
	{

	}

	void Window::Init()
	{
		owned_widget_ = ui::Widget::Create();
		owned_widget_->SetMessageHanler(this);

		view_ = new WindowHostView(this);
		mouse_event_handler_.reset(new MouseEventHandler(this));
		//focus_manager_.reset(new FocusManager);
		SetCursor(::LoadCursor(NULL, IDC_ARROW));
	}

	void Window::SetBounds(int x, int y, int width, int height)
	{
		if (owned_widget_)
			owned_widget_->SetBounds(Rect(x, y, width, height));
	}

	Rect Window::GetBounds()
	{
		if (owned_widget_)
			return owned_widget_->GetWindowScreenBounds();
		return Rect();
	}

	void Window::SetSize(int w, int h)
	{
		if (owned_widget_)
			owned_widget_->SetSize(Size(w, h));
	}

	Size Window::GetSize()
	{
		if (owned_widget_)
			return owned_widget_->GetWindowScreenBounds().size(); \
		return Size();
	}

	void Window::CenterWindow()
	{
		if (!owned_widget_)
			return;

		owned_widget_->CenterWindow(GetSize());
	}

	void Window::Close()
	{
		widget()->Close();
	}

	


	BOOL Window::ProcessWindowMessage(HWND window, UINT message, WPARAM w_param, LPARAM l_param, LRESULT& result)
	{
		if (message == WM_PAINT)
		{
			Painter painter(widget());
			GetHostView()->DoPaint(&painter);
			return TRUE;
		}
		else if ((message >= WM_MOUSEFIRST && message <= WM_MOUSELAST)
			|| (message >= WM_NCMOUSEMOVE && message <= WM_NCXBUTTONDBLCLK)
			|| message == WM_MOUSELEAVE
			|| message == WM_NCMOUSELEAVE)
		{
			mouse_event_handler_->HandleMouseEvent(message, w_param, l_param);
			return TRUE;
		}
		else if (message == WM_KEYDOWN
			|| message == WM_KEYUP
			|| message == WM_SYSKEYDOWN
			|| message == WM_SYSKEYUP
			|| message == WM_CHAR)
		{
			//result = HandleKeyEvent(message, w_param, l_param);
			return TRUE;
		}
		else if (message == WM_SETFOCUS)
		{
			OnSetFocus();
		}
		else if (message == WM_KILLFOCUS)
		{
			if (focused_view_)
			{
				//focused_view_->Lo
			}
		}
		return FALSE;
	}

	View* Window::GetFocusedView() const
	{
		return focused_view_;
	}

	void Window::OnSetFocus()
	{

	}

	void Window::SchedulePaint(const Rect& r)
	{
		widget()->InvalidateRect(r);
	}

	/*void WidgetView::SchedulePaintInRect(const Rect& r)
	{
		if (!visible_ || !painting_enabled_)
			return;

		Rect invalid_rect(GetLocalBounds());
		invalid_rect.Intersect(r);
		if (!invalid_rect.IsEmpty())
			widget()->InvalidateRect(invalid_rect);
	}

	EventDispatcher* WidgetView::GetEventDispatcher() const
	{
		return dispatcher_.get();
	}

	

	View* WidgetView::GetFocusedView() const
	{
		return focus_manager_.get() ? focus_manager_->GetFocusedView()
			: NULL;
	}

	FocusManager* WidgetView::GetFocusManager() const
	{
		return focus_manager_.get();
	}*/


}