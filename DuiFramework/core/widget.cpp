#include "stdafx.h"
#include "widget.h"

#include "utils/utils.h"

#include <cassert>

namespace ui
{

	const wchar_t* const kWidgetClassName = L"DuiFramework.Widget";

	static const DWORD kWindowDefaultChildStyle =
		WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	static const DWORD kWindowDefaultStyle = WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_OVERLAPPED;
	static const DWORD kWindowDefaultExStyle = WS_EX_WINDOWEDGE;

	Widget::Widget()
		: hwnd_(NULL)
		, window_style_(0)
		, window_ex_style_(kWindowDefaultExStyle)
		, previous_cursor_(NULL)
		, class_style_(CS_DBLCLKS)
	{

	}

	Widget::~Widget()
	{
		if (::IsWindow(hwnd_)) {
			::SetWindowLongPtr(hwnd_, GWLP_USERDATA, NULL);
		}
	}

	Widget* Widget::Create(Widget* parent, const Rect& bounds)
	{
		Widget* widget = new Widget;
		if (parent)
			widget->Init(parent->hwnd(), bounds);
		else
			widget->Init(NULL, bounds);
		return widget;
	}

	Widget* Widget::Create()
	{
		return Create(NULL, Rect());
	}

	Widget* Widget::GetWidgetForHWND(HWND hwnd)
	{
		return reinterpret_cast<Widget*>(::GetWindowLongPtr(hwnd, GWLP_USERDATA));
	}

	void Widget::InitClass()
	{
		WNDCLASSEX wc = { 0 };
		wc.cbSize = sizeof(wc);
		wc.lpfnWndProc = WndProc;
		wc.hInstance = GetModuleHandle(NULL);
		wc.lpszClassName = kWidgetClassName;
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_DROPSHADOW;
		wc.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(COLOR_BACKGROUND));
		RegisterClassEx(&wc);
	}

	void Widget::Init(HWND parent, const Rect& bounds)
	{
		if (window_style_ == 0)
			window_style_ = parent ? kWindowDefaultChildStyle : kWindowDefaultStyle;

		int x, y, width, height;
		if (bounds.IsEmpty()) 
		{
			x = y = width = height = CW_USEDEFAULT;
		}
		else 
		{
			x = bounds.x();
			y = bounds.y();
			width = bounds.width();
			height = bounds.height();
		}

		bool destroyed = false;
		HWND hwnd = CreateWindowEx(window_ex_style_, kWidgetClassName, NULL,
			window_style_, x, y, width, height,
			parent, NULL, NULL, this);

		assert(hwnd_ && GetLastError() == 0);

		DWORD dwStyle = GetWindowLong(GWL_STYLE);
		SetWindowLong(GWL_STYLE, (dwStyle & ~WS_CAPTION));
	}

	void Widget::SetMessageHanler(MessageHandler* handler)
	{
		handler_ = handler;
	}

	Widget::MessageHandler* Widget::GetMessageHanler()
	{
		return handler_;
	}

	Rect Widget::GetWindowScreenBounds() const
	{
		RECT r;
		::GetWindowRect(hwnd(), &r);
		return Rect(r);
	}

	/*Rect Widget::GetRestoredBounds() const
	{

	}*/

	void Widget::SetBounds(const Rect& bounds)
	{
		LONG style = GetWindowLong(GWL_STYLE);
		if (style & WS_MAXIMIZE)
			SetWindowLong(GWL_STYLE, style & ~WS_MAXIMIZE);
		SetWindowPos(NULL, bounds.x(), bounds.y(), bounds.width(), bounds.height(),
			SWP_NOACTIVATE | SWP_NOZORDER);
	}

	void Widget::SetSize(const Size& size)
	{
		SetWindowPos(NULL, 0, 0, size.width(), size.height(),
			SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOMOVE);
	}

	void Widget::CenterWindow(const Size& size)
	{
		HWND parent = GetParent();
		if (!IsWindow())
			parent = ::GetWindow(hwnd(), GW_OWNER);
		CenterAndSizeWindow(parent, hwnd(), size);
	}

	void Widget::Close()
	{
		if (!IsWindow())
			return;  // No need to do anything.

		// Let's hide ourselves right away.
		Hide();

		::DestroyWindow(hwnd());
	}

	void Widget::Show(int state)
	{
		::ShowWindow(hwnd_, state);
	}

	bool Widget::IsWindow()
	{
		return ::IsWindow(hwnd_) == TRUE;
	}

	HWND Widget::GetParent() const {
		return ::GetParent(hwnd());
	}

	LONG Widget::GetWindowLong(int index)
	{
		assert(::IsWindow(hwnd()));
		return ::GetWindowLong(hwnd(), index);
	}

	BOOL Widget::GetWindowRect(RECT* rect) const
	{
		return ::GetWindowRect(hwnd(), rect);
	}

	LONG Widget::SetWindowLong(int index, LONG new_long)
	{
		assert(::IsWindow(hwnd()));
		return ::SetWindowLong(hwnd(), index, new_long);
	}

	BOOL Widget::SetWindowPos(HWND hwnd_after, int x, int y, int cx, int cy, UINT flags)
	{
		assert(::IsWindow(hwnd()));
		return ::SetWindowPos(hwnd(), hwnd_after, x, y, cx, cy, flags);
	}

	BOOL Widget::SetWindowPos(int x, int y)
	{
		return SetWindowPos(NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	}

	BOOL Widget::MoveWindow(int x, int y, int width, int height)
	{
		return MoveWindow(x, y, width, height, TRUE);
	}

	BOOL Widget::MoveWindow(int x, int y, int width, int height, BOOL repaint)
	{
		assert(::IsWindow(hwnd()));
		return ::MoveWindow(hwnd(), x, y, width, height, repaint);
	}

	int Widget::SetWindowRgn(HRGN region, BOOL redraw)
	{
		assert(::IsWindow(hwnd()));
		return ::SetWindowRgn(hwnd(), region, redraw);
	}

	BOOL Widget::GetClientRect(RECT* rect) const
	{
		assert(::IsWindow(hwnd()));
		return ::GetClientRect(hwnd(), rect);
	}

	void Widget::Hide()
	{
		if (IsWindow()) {
			// NOTE: Be careful not to activate any windows here (for example, calling
			// ShowWindow(SW_HIDE) will automatically activate another window).  This
			// code can be called while a window is being deactivated, and activating
			// another window will screw up the activation that is already in progress.
			SetWindowPos(NULL, 0, 0, 0, 0,
				SWP_HIDEWINDOW | SWP_NOACTIVATE | SWP_NOMOVE |
				SWP_NOREPOSITION | SWP_NOSIZE | SWP_NOZORDER);

			//if (!GetParent())
			//	NotifyOwnedWindowsParentClosing();
		}
	}

	void Widget::Active()
	{
		if (IsMinimized())
			::ShowWindow(hwnd(), SW_RESTORE);
		::SetWindowPos(hwnd(), HWND_TOP, 0, 0, 0, 0,
			SWP_NOSIZE | SWP_NOMOVE);
		::SetForegroundWindow(hwnd());
	}

	bool Widget::IsActive() const
	{
		return ::GetActiveWindow() == hwnd();
	}

	void Widget::Maximize()
	{
		::SendMessage(hwnd(), WM_SYSCOMMAND, SC_MAXIMIZE, 0);
	}

	void Widget::Minimize()
	{
		::SendMessage(hwnd(), WM_SYSCOMMAND, SC_MINIMIZE, 0);
	}

	void Widget::Restore()
	{
		::SendMessage(hwnd(), WM_SYSCOMMAND, SC_RESTORE, 0);
	}

	bool Widget::IsMaximized() const
	{
		return !!::IsZoomed(hwnd());
	}

	bool Widget::IsMinimized() const
	{
		return !!::IsIconic(hwnd());
	}

	bool Widget::IsVisible() const
	{
		return !!::IsWindowVisible(hwnd());
	}

	LRESULT Widget::OnWndProc(UINT message, WPARAM w_param, LPARAM l_param)
	{
		LRESULT result = 0;

		// Handle the message if it's in our message map; otherwise, let the system
		// handle it.
		if (!ProcessWindowMessage(hwnd_, message, w_param, l_param, result))
			result = DefWindowProc(hwnd_, message, w_param, l_param);

		return result;
	}

	LRESULT CALLBACK Widget::WndProc(HWND hwnd, UINT message, WPARAM w_param, LPARAM l_param)
	{
		if (message == WM_NCCREATE) {
			CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(l_param);
			Widget* window = reinterpret_cast<Widget*>(cs->lpCreateParams);
			assert(window);
			::SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
			window->hwnd_ = hwnd;
			return TRUE;
		}

		Widget* window = reinterpret_cast<Widget*>(
			::GetWindowLongPtr(hwnd, GWLP_USERDATA));
		if (!window)
			return 0;

		return window->OnWndProc(message, w_param, l_param);
	}

	BOOL Widget::ProcessWindowMessage(HWND window, UINT message, WPARAM w_param, LPARAM l_param, LRESULT& result)
	{
		if (handler_)
			return handler_->ProcessWindowMessage(window, message, w_param, l_param, result);
		return FALSE;
	}

	void Widget::SetCursor(HCURSOR cursor)
	{
		if (cursor) 
		{
			previous_cursor_ = ::SetCursor(cursor);
		}
		else if (previous_cursor_) 
		{
			::SetCursor(previous_cursor_);
			previous_cursor_ = NULL;
		}
	}

	void Widget::InvalidateRect(const Rect& r)
	{
		::InvalidateRect(hwnd_, &r.ToRECT(), FALSE);
	}

	void Widget::SetFocus()
	{
		::SetFocus(hwnd_);
	}

	bool Widget::IsFocused()
	{
		return ::GetFocus() == hwnd_;
	}


	

}