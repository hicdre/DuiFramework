#include "stdafx.h"
#include "ui_platform_window.h"
#include "uikit/2D/win32_port.h"

//#include "utils/utils.h"

#include <cassert>

namespace ui
{

	const wchar_t* const kWidgetClassName = L"DuiFramework.Widget";

	static const DWORD kWindowDefaultChildStyle =
		WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	static const DWORD kWindowDefaultStyle = WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_OVERLAPPED;
	static const DWORD kWindowDefaultExStyle = WS_EX_WINDOWEDGE;

	UIPlatformWindow::UIPlatformWindow()
		: hwnd_(NULL)
		, window_style_(0)
		, window_ex_style_(kWindowDefaultExStyle)
		, previous_cursor_(NULL)
		, class_style_(CS_DBLCLKS)
	{

	}

	UIPlatformWindow::~UIPlatformWindow()
	{
		if (::IsWindow(hwnd_)) {
			::SetWindowLongPtr(hwnd_, GWLP_USERDATA, NULL);
		}
	}

// 	Widget* UIPlatformWindow::Create(Widget* parent, const Rect& bounds)
// 	{
// 		Widget* widget = new Widget;
// 		if (parent)
// 			widget->Init(parent->hwnd(), bounds);
// 		else
// 			widget->Init(NULL, bounds);
// 		return widget;
// 	}
// 
// 	Widget* Widget::Create()
// 	{
// 		return Create(NULL, Rect());
// 	}

	UIPlatformWindow* UIPlatformWindow::GetPlatformWindow(HWND hwnd)
	{
		return reinterpret_cast<UIPlatformWindow*>(::GetWindowLongPtr(hwnd, GWLP_USERDATA));
	}

	void UIPlatformWindow::InitClass()
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

	void UIPlatformWindow::Init(HWND parent, const Rect& bounds)
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

		//assert(hwnd_ && GetLastError() == 0);

		DWORD dwStyle = GetWindowLong(GWL_STYLE);
		SetWindowLong(GWL_STYLE, (dwStyle & ~WS_CAPTION));
	}

	void UIPlatformWindow::SetMessageHanler(MessageHandler* handler)
	{
		handler_ = handler;
	}

	UIPlatformWindow::MessageHandler* UIPlatformWindow::GetMessageHanler()
	{
		return handler_;
	}

	Rect UIPlatformWindow::GetWindowScreenBounds() const
	{
		RECT r;
		::GetWindowRect(hwnd(), &r);
		return UICreateRect(r);
	}

	/*Rect Widget::GetRestoredBounds() const
	{

	}*/

	void UIPlatformWindow::SetBounds(const Rect& bounds)
	{
		LONG style = GetWindowLong(GWL_STYLE);
		if (style & WS_MAXIMIZE)
			SetWindowLong(GWL_STYLE, style & ~WS_MAXIMIZE);
		SetWindowPos(NULL, bounds.x(), bounds.y(), bounds.width(), bounds.height(),
			SWP_NOACTIVATE | SWP_NOZORDER);
	}

	void UIPlatformWindow::SetSize(const Size& size)
	{
		SetWindowPos(NULL, 0, 0, size.width(), size.height(),
			SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOMOVE);
	}

	void UIPlatformWindow::CenterWindow(const Size& size)
	{
		HWND parent = GetParent();
		if (!IsWindow())
			parent = ::GetWindow(hwnd(), GW_OWNER);
		CenterAndSizeWindow(parent, hwnd(), size);
	}

	void UIPlatformWindow::Close()
	{
		if (!IsWindow())
			return;  // No need to do anything.

		// Let's hide ourselves right away.
		Hide();

		::DestroyWindow(hwnd());
	}

	void UIPlatformWindow::Show(int state)
	{
		::ShowWindow(hwnd_, state);
	}

	bool UIPlatformWindow::IsWindow()
	{
		return ::IsWindow(hwnd_) == TRUE;
	}

	HWND UIPlatformWindow::GetParent() const {
		return ::GetParent(hwnd());
	}

	LONG UIPlatformWindow::GetWindowLong(int index)
	{
		assert(::IsWindow(hwnd()));
		return ::GetWindowLong(hwnd(), index);
	}

	BOOL UIPlatformWindow::GetWindowRect(RECT* rect) const
	{
		return ::GetWindowRect(hwnd(), rect);
	}

	LONG UIPlatformWindow::SetWindowLong(int index, LONG new_long)
	{
		assert(::IsWindow(hwnd()));
		return ::SetWindowLong(hwnd(), index, new_long);
	}

	BOOL UIPlatformWindow::SetWindowPos(HWND hwnd_after, int x, int y, int cx, int cy, UINT flags)
	{
		assert(::IsWindow(hwnd()));
		return ::SetWindowPos(hwnd(), hwnd_after, x, y, cx, cy, flags);
	}

	BOOL UIPlatformWindow::SetWindowPos(int x, int y)
	{
		return SetWindowPos(NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	}

	BOOL UIPlatformWindow::MoveWindow(int x, int y, int width, int height)
	{
		return MoveWindow(x, y, width, height, TRUE);
	}

	BOOL UIPlatformWindow::MoveWindow(int x, int y, int width, int height, BOOL repaint)
	{
		assert(::IsWindow(hwnd()));
		return ::MoveWindow(hwnd(), x, y, width, height, repaint);
	}

	int UIPlatformWindow::SetWindowRgn(HRGN region, BOOL redraw)
	{
		assert(::IsWindow(hwnd()));
		return ::SetWindowRgn(hwnd(), region, redraw);
	}

	BOOL UIPlatformWindow::GetClientRect(RECT* rect) const
	{
		assert(::IsWindow(hwnd()));
		return ::GetClientRect(hwnd(), rect);
	}

	void UIPlatformWindow::Hide()
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

	void UIPlatformWindow::Active()
	{
		if (IsMinimized())
			::ShowWindow(hwnd(), SW_RESTORE);
		::SetWindowPos(hwnd(), HWND_TOP, 0, 0, 0, 0,
			SWP_NOSIZE | SWP_NOMOVE);
		::SetForegroundWindow(hwnd());
	}

	bool UIPlatformWindow::IsActive() const
	{
		return ::GetActiveWindow() == hwnd();
	}

	void UIPlatformWindow::Maximize()
	{
		::SendMessage(hwnd(), WM_SYSCOMMAND, SC_MAXIMIZE, 0);
	}

	void UIPlatformWindow::Minimize()
	{
		::SendMessage(hwnd(), WM_SYSCOMMAND, SC_MINIMIZE, 0);
	}

	void UIPlatformWindow::Restore()
	{
		::SendMessage(hwnd(), WM_SYSCOMMAND, SC_RESTORE, 0);
	}

	bool UIPlatformWindow::IsMaximized() const
	{
		return !!::IsZoomed(hwnd());
	}

	bool UIPlatformWindow::IsMinimized() const
	{
		return !!::IsIconic(hwnd());
	}

	bool UIPlatformWindow::IsVisible() const
	{
		return !!::IsWindowVisible(hwnd());
	}

	LRESULT UIPlatformWindow::OnWndProc(UINT message, WPARAM w_param, LPARAM l_param)
	{
		LRESULT result = 0;

		// Handle the message if it's in our message map; otherwise, let the system
		// handle it.
		if (!ProcessWindowMessage(hwnd_, message, w_param, l_param, result))
			result = DefWindowProc(hwnd_, message, w_param, l_param);

		return result;
	}

	LRESULT CALLBACK UIPlatformWindow::WndProc(HWND hwnd, UINT message, WPARAM w_param, LPARAM l_param)
	{
		if (message == WM_NCCREATE) {
			CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(l_param);
			UIPlatformWindow* window = reinterpret_cast<UIPlatformWindow*>(cs->lpCreateParams);
			assert(window);
			::SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
			window->hwnd_ = hwnd;
			return TRUE;
		}

		UIPlatformWindow* window = reinterpret_cast<UIPlatformWindow*>(
			::GetWindowLongPtr(hwnd, GWLP_USERDATA));
		if (!window)
			return 0;

		return window->OnWndProc(message, w_param, l_param);
	}

	BOOL UIPlatformWindow::ProcessWindowMessage(HWND window, UINT message, WPARAM w_param, LPARAM l_param, LRESULT& result)
	{
		if (handler_)
			return handler_->ProcessWindowMessage(window, message, w_param, l_param, result);
		return FALSE;
	}

	void UIPlatformWindow::SetCursor(HCURSOR cursor)
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

	void UIPlatformWindow::InvalidateRect(const Rect& r)
	{
		::InvalidateRect(hwnd_, &UICreateRECT(r), FALSE);
	}

	void UIPlatformWindow::SetFocus()
	{
		::SetFocus(hwnd_);
	}

	bool UIPlatformWindow::IsFocused()
	{
		return ::GetFocus() == hwnd_;
	}

	void UIPlatformWindow::SetCaptured()
	{
		if (::GetCapture() != hwnd_)
			::SetCapture(hwnd_);
	}

	void UIPlatformWindow::ReleaseCaptured()
	{
		::ReleaseCapture();
	}


	

}