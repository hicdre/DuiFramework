#include "stdafx.h"
#include "window.h"

#include "render/painter.h"
#include "core/view.h"

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

		//mouse_event_handler_.reset(new MouseEventHandler(this));
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

	void Window::SchedulePaintInRect(const Rect& r)
	{
		if (!visible_ || !painting_enabled_)
			return;

		Rect invalid_rect(GetLocalBounds());
		invalid_rect.Intersect(r);
		if (!invalid_rect.IsEmpty())
			widget()->InvalidateRect(invalid_rect);
	}

	/*EventDispatcher* WidgetView::GetEventDispatcher() const
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

	BOOL Window::ProcessWindowMessage(HWND window, UINT message, WPARAM w_param, LPARAM l_param, LRESULT& result)
	{
		if (message == WM_PAINT)
		{
			Painter painter(widget());
			DoPaint(&painter);
			return TRUE;
		}
		else if ((message >= WM_MOUSEFIRST && message <= WM_MOUSELAST)
			|| (message >= WM_NCMOUSEMOVE && message <= WM_NCXBUTTONDBLCLK)
			|| message == WM_MOUSELEAVE
			|| message == WM_NCMOUSELEAVE)
		{
			ProcessMouseMessage(message, w_param, l_param);
			return TRUE;
		}
		else if (message == WM_KEYDOWN
			|| message == WM_KEYUP
			|| message == WM_SYSKEYDOWN
			|| message == WM_SYSKEYUP
			|| message == WM_CHAR)
		{
			ProcessKeyMessage(message, w_param, l_param);
			return TRUE;
		}
		else if (message == WM_SETFOCUS)
		{
			SetFocus(this);
		}
		else if (message == WM_KILLFOCUS)
		{
			SetFocus(NULL);
		}
		return FALSE;
	}

	void Window::ProcessMouseMessage(UINT message, WPARAM w_param, LPARAM l_param)
	{
		Point pt = GetMousePosition(widget());

		View* old_view = hitttest_view_;
		hitttest_view_ = Hittest(pt);

		if (hitttest_view_) {
			//ÉèÖÃÊó±êÖ¸Õë
			HCURSOR cursor = hitttest_view_->GetCursor();
			widget()->SetCursor(cursor);

			MouseEvent evt(EVENT_MOUSE_MOVE, pt, hitttest_view_);
			hitttest_view_->HandleEvent(&evt);
		}

		if (old_view != hitttest_view_)
		{
			if (old_view == NULL && hitttest_view_ != NULL) {
				DispatchMouseEnterEvent(old_view, hitttest_view_, pt);

				TRACKMOUSEEVENT tme = { 0 };
				tme.cbSize = sizeof(TRACKMOUSEEVENT);
				tme.dwFlags = TME_LEAVE;
				tme.hwndTrack = widget()->hwnd();
				tme.dwHoverTime = HOVER_DEFAULT;
				::TrackMouseEvent(&tme);
				return;
			}
			else if (old_view != NULL && hitttest_view_ == NULL) 
			{
				DispatchMouseLeaveEvent(old_view, hitttest_view_, pt);
			}
			else
			{
				View* public_ancestor = hitttest_view_->GetAncestorTo(old_view);
				if (public_ancestor)
				{
					DispatchMouseLeaveEvent(old_view, public_ancestor, pt);
					DispatchMouseEnterEvent(public_ancestor, hitttest_view_, pt);
				}
			}
		}

		MouseEvent evt(EVENT_UNKNOWN, pt, hitttest_view_);
		switch (message)
		{
		case WM_LBUTTONDOWN:
		case WM_NCLBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_NCMBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_NCRBUTTONDOWN:
			evt.SetEventType(EVENT_MOUSE_DOWN);
			break;
		case WM_XBUTTONDOWN:
		case WM_NCXBUTTONDOWN:
			//DispatchMouseDownEvent(hitttest_view_, MOUSE_LEFT);
			break;
		case WM_LBUTTONUP:
		case WM_NCLBUTTONUP:
		case WM_MBUTTONUP:
		case WM_NCMBUTTONUP:
		case WM_RBUTTONUP:
		case WM_NCRBUTTONUP:
			evt.SetEventType(EVENT_MOUSE_UP);
			break;
		case WM_XBUTTONUP:
		case WM_NCXBUTTONUP:
			//DispatchMouseUpEvent(hitttest_view_);
			break;
		case WM_LBUTTONDBLCLK:
		case WM_NCLBUTTONDBLCLK:
		case WM_MBUTTONDBLCLK:
		case WM_NCMBUTTONDBLCLK:
		case WM_RBUTTONDBLCLK:
		case WM_NCRBUTTONDBLCLK:
			evt.SetEventType(EVENT_MOUSE_DBCLICK);
			break;
		case WM_XBUTTONDBLCLK:
		case WM_NCXBUTTONDBLCLK:
			//DispatchMouseDbClickEvent(hitttest_view_);
			break;
		default:
			break;
		}

		if (evt.IsMouseEvent())
			hitttest_view_->HandleEvent(&evt);
	}

	void Window::DispatchMouseLeaveEvent(View* from, View* to, const Point& pt)
	{
		if (to == NULL)
			to = this;

		for (View* v = from; v != to; v = v->parent())
		{
			MouseEvent evt(EVENT_MOUSE_LEAVE, pt, v);
			v->HandleEvent(&evt);
		}
		MouseEvent evt(EVENT_MOUSE_LEAVE, pt, to);
		to->HandleEvent(&evt);
	}

	void Window::DispatchMouseEnterEvent(View* from, View* to, const Point& pt)
	{
		Views views;
		if (from == NULL)
			from = this;
		for (View* v = to; v != from; v = v->parent())
			views.push_back(v);
		views.push_back(from);

		for (auto iter = views.rbegin(); iter != views.rend(); iter++)
		{
			MouseEvent evt(EVENT_MOUSE_ENTER, pt, *iter);
			(*iter)->HandleEvent(&evt);
		}
	}

	void Window::ProcessKeyMessage(UINT message, WPARAM w_param, LPARAM l_param)
	{
		if (!focused_view_)
			return;

		KeyEvent evt(EVENT_UNKNOWN, w_param, focused_view_, l_param);
		switch (message)
		{
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			evt.SetEventType(EVENT_KEY_PRESSED);
			break;
		case WM_KEYUP:
		case WM_SYSKEYUP:
			evt.SetEventType(EVENT_KEY_RELEASED);
			break;
		case WM_CHAR:
			assert(0);
			break;
		default:
			break;
		}
		if (evt.IsKeyEvent())
			focused_view_->HandleEvent(&evt);
	}

	FocusManager* Window::GetFocusManager() const
	{
		return const_cast<FocusManager*>(static_cast<const FocusManager*>(this));
	}

	void Window::SetFocus(View* v)
	{
		if (focused_view_ == v)
			return;

		if (v && !v->IsFocusable())
			return;

		View* old_view = focused_view_;
		focused_view_ = v;

		if (old_view)
		{
			FocusEvent evt(EVENT_LOSE_FOCUS, old_view, focused_view_);
			focused_view_->HandleEvent(&evt);
		}
		if (focused_view_)
		{
			if (!widget()->IsFocused())
			{
				widget()->SetFocus();
			}
			FocusEvent evt(EVENT_GAIN_FOCUS, old_view, focused_view_);
			focused_view_->HandleEvent(&evt);
		}
	}


}