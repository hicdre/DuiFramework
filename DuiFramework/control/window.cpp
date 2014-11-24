#include "stdafx.h"
#include "window.h"

#include "render/render_context.h"
#include "utils/utils.h"
#include "third_party/tinyxml2.h"
#include "window_builder.h"
#include "utils/resource_loader.h"

namespace ui
{
	Window::Window()
	{
	}

	Window::~Window()
	{
		if (owned_widget_)
		{
			delete owned_widget_;
			owned_widget_ = NULL;
		}
		
	}

	void Window::AttachWidget(Widget* widget)
	{
		owned_widget_ = widget;
		owned_widget_->SetMessageHanler(this);

		SetCursor(::LoadCursor(NULL, IDC_ARROW));

		//先同步为widget大小
		SetWindowSize(layoutWidth(), layoutHeight());
	}

	Widget* Window::DetachWidget()
	{
		Widget* w = owned_widget_;
		owned_widget_ = NULL;
		return w;
	}

	void Window::SetWindowBounds(int x, int y, int width, int height)
	{
		if (owned_widget_)
			owned_widget_->SetBounds(Rect(x, y, width, height));
	}

	Rect Window::GetWindowBounds()
	{
		if (owned_widget_)
			return owned_widget_->GetWindowScreenBounds();
		return Rect();
	}

	void Window::SetWindowSize(int w, int h)
	{
		if (owned_widget_)
			owned_widget_->SetSize(Size(w, h));
	}

	Size Window::GetWindowSize()
	{
		if (owned_widget_)
			return owned_widget_->GetWindowScreenBounds().size();
		return Size();
	}

	void Window::CenterWindow()
	{
		if (!owned_widget_)
			return;

		owned_widget_->CenterWindow(GetWindowSize());
	}

	void Window::Close()
	{
		widget()->Close();
	}

	void Window::SchedulePaint(const Rect& r)
	{
		widget()->InvalidateRect(r);
	}

	void Window::OnChildSchedulePaintInRect(View* child, const Rect& r)
	{
		if (!visible_)
			return;

		Rect invalid_rect = ConvertRectFromChild(child, r).Intersect(GetLocalBounds());
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
			RenderContext painter(widget());
			DoPaint(&painter, GetLocalBounds());
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
			//ProcessKeyMessage(message, w_param, l_param);
			return TRUE;
		}
		else if (message == WM_SETFOCUS)
		{
			//SetFocus(this);
			return TRUE;
		}
		else if (message == WM_KILLFOCUS)
		{
			SetFocus(NULL);
			return TRUE;
		}
		else if (message == WM_SIZE)
		{
			Size sz = { LOWORD(l_param), HIWORD(l_param) };
			SetSize(sz);
			return TRUE;
		}
		return FALSE;
	}

	void Window::Load(const std::string& str)
	{
		tinyxml2::XMLDocument xml;
		if (tinyxml2::XML_SUCCESS == xml.Parse(str.c_str()))
		{
			WindowBuilder builder(this, &xml);
			builder.Run();
		}
	}

	void Window::LoadFile( const std::wstring& file )
	{
		tinyxml2::XMLDocument xml;
		if (tinyxml2::XML_SUCCESS == xml.LoadFile(WideToMultiByte(
			ResourceLoader::Get()->ResolvePath(file)).c_str()))
		{
			WindowBuilder builder(this, &xml);
			builder.Run();
		}
	}

	void Window::AddStyleSheet( StyleSheet* sheet )
	{
		style_sheets_.AddStyleSheet(sheet);
	}

	const StyleSheetList* Window::GetStyleSheetList() const
	{
		return &style_sheets_;
	}

	const char* Window::tag() const 
	{
		return "Window";
	}


	void Window::ProcessMouseMessage(UINT message, WPARAM w_param, LPARAM l_param)
	{
		widget()->SetCursor(GetCursor());
#if 0
		Point pt = GetMousePosition(widget());

		View* old_view = hittest_view_;
		if (captured_view_)
		{
			hittest_view_ = captured_view_->Hittest(ConvertPointFromWidget(pt));
			if (hittest_view_ == NULL)
				hittest_view_ = captured_view_;
		}
		else
		{
			hittest_view_ = Hittest(pt);
		}

		if (hittest_view_) {
			//设置鼠标指针
			HCURSOR cursor = hittest_view_->GetCursor();
			widget()->SetCursor(cursor);

			MouseEvent evt(captured_view_ ? EVENT_DRAG_MOVE : EVENT_MOUSE_MOVE, 
				pt, hittest_view_);
			hittest_view_->HandleEvent(&evt);
		}

		if (old_view != hittest_view_)
		{
			if (old_view == NULL && hittest_view_ != NULL) {
				DispatchMouseEnterEvent(old_view, hittest_view_, pt);

				TRACKMOUSEEVENT tme = { 0 };
				tme.cbSize = sizeof(TRACKMOUSEEVENT);
				tme.dwFlags = TME_LEAVE;
				tme.hwndTrack = widget()->hwnd();
				tme.dwHoverTime = HOVER_DEFAULT;
				::TrackMouseEvent(&tme);
				return;
			}
			else if (old_view != NULL && hittest_view_ == NULL) 
			{
				DispatchMouseLeaveEvent(old_view, hittest_view_, pt);
			}
			else
			{
				View* public_ancestor = hittest_view_->GetAncestorTo(old_view);
				if (public_ancestor)
				{
					DispatchMouseLeaveEvent(old_view, public_ancestor, pt);
					DispatchMouseEnterEvent(public_ancestor, hittest_view_, pt);
				}
			}
		}
		MouseEvent evt(EVENT_UNKNOWN, pt, hittest_view_);
		switch (message)
		{
		case WM_LBUTTONDOWN:
		case WM_NCLBUTTONDOWN:
			captured_view_ = hittest_view_->GetDragableView();
			if (captured_view_)
			{
				MouseEvent evt1(EVENT_DRAG_BEGIN, pt, captured_view_);
				captured_view_->HandleEvent(&evt1);
			}
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
			if (captured_view_)
			{
				View* last_captured_view = captured_view_;
				captured_view_ = NULL;
				MouseEvent evt1(EVENT_DRAG_END, pt, last_captured_view);
				last_captured_view->HandleEvent(&evt1);
			}
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
			hittest_view_->HandleEvent(&evt);
#endif
	}
#if 0
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
			std::cout << "WM_KEYDOWN: " << w_param << std::endl;
			break;
		case WM_KEYUP:
		case WM_SYSKEYUP:
			evt.SetEventType(EVENT_KEY_RELEASED);
			std::cout << "WM_KEYUP: " << w_param << std::endl;
			break;
		case WM_CHAR:
			evt.SetEventType(EVENT_CHAR);
			std::cout << "WM_CHAR: " << w_param << std::endl;
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
			old_view->HandleEvent(&evt);
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
#endif

}