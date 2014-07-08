#include "stdafx.h"
#include "event.h"

#include "core/view.h"
#include "core/app.h"
#include "core/widget.h"

namespace ui
{
	bool IsButtonDown(const MSG& msg) {
		return ((MK_LBUTTON | MK_MBUTTON | MK_RBUTTON | MK_XBUTTON1 | MK_XBUTTON2) &
			msg.wParam) != 0;
	}

	int GetMouseKey(const MSG& msg) {
		switch (msg.message) {
		case WM_LBUTTONDBLCLK:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_NCLBUTTONDBLCLK:
		case WM_NCLBUTTONDOWN:
		case WM_NCLBUTTONUP:
			return MK_LBUTTON;
		case WM_MBUTTONDBLCLK:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_NCMBUTTONDBLCLK:
		case WM_NCMBUTTONDOWN:
		case WM_NCMBUTTONUP:
			return MK_MBUTTON;
		case WM_RBUTTONDBLCLK:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_NCRBUTTONDBLCLK:
		case WM_NCRBUTTONDOWN:
		case WM_NCRBUTTONUP:
			return MK_RBUTTON;
		case WM_NCXBUTTONDBLCLK:
		case WM_NCXBUTTONDOWN:
		case WM_NCXBUTTONUP:
		case WM_XBUTTONDBLCLK:
		case WM_XBUTTONDOWN:
		case WM_XBUTTONUP:
			return MK_XBUTTON1;
		}
		return 0;
	}

	bool IsClientMouseEvent(const MSG& msg) {
		return msg.message == WM_MOUSELEAVE ||
			msg.message == WM_MOUSEHOVER ||
			(msg.message >= WM_MOUSEFIRST &&
			msg.message <= WM_MOUSELAST);
	}

	bool IsNonClientMouseEvent(const MSG& msg) {
		return msg.message == WM_NCMOUSELEAVE ||
			msg.message == WM_NCMOUSEHOVER ||
			(msg.message >= WM_NCMOUSEMOVE &&
			msg.message <= WM_NCXBUTTONDBLCLK);
	}

	bool IsMouseEvent(const MSG& msg) {
		return IsClientMouseEvent(msg) ||
			IsNonClientMouseEvent(msg);
	}

	bool IsMouseWheelEvent(const MSG& msg) {
		return msg.message == WM_MOUSEWHEEL ||
			msg.message == WM_MOUSEHWHEEL;
	}

	bool IsKeyEvent(const MSG& msg) {
		return msg.message == WM_KEYDOWN ||
			msg.message == WM_SYSKEYDOWN ||
			msg.message == WM_CHAR ||
			msg.message == WM_KEYUP ||
			msg.message == WM_SYSKEYUP;
	}

	Point GetMousePosition(Widget* v)
	{
		POINT pt;
		::GetCursorPos(&pt);
		::ScreenToClient(v->hwnd(), &pt);
		return Point(pt);
	}

	bool IsShiftPressed() {
		return (::GetKeyState(VK_SHIFT) & 0x8000) == 0x8000;
	}

	bool IsCtrlPressed() {
		return (::GetKeyState(VK_CONTROL) & 0x8000) == 0x8000;
	}

	bool IsAltPressed() {
		return (::GetKeyState(VK_MENU) & 0x8000) == 0x8000;
	}

	bool IsAltGrPressed() {
		return (::GetKeyState(VK_MENU) & 0x8000) == 0x8000 &&
			(::GetKeyState(VK_CONTROL) & 0x8000) == 0x8000;
	}

	bool IsLeftMousePressed() {
		return (::GetKeyState(VK_LBUTTON) & 0x8000) == 0x8000;
	}

	bool IsRightMousePressed() {
		return (::GetKeyState(VK_RBUTTON) & 0x8000) == 0x8000;
	}

	bool IsMiddleMousePressed() {
		return (::GetKeyState(VK_MBUTTON) & 0x8000) == 0x8000;
	}

	int GetMouseKeyFlags() {
		int flags = 0;
		flags |= IsAltPressed() ? KEY_ALT : FLAG_NONE;
		flags |= IsShiftPressed() ? KEY_SHIFT : FLAG_NONE;
		flags |= IsCtrlPressed() ? KEY_CONTROL : FLAG_NONE;
		flags |= IsLeftMousePressed() ? MOUSE_LEFT : FLAG_NONE;
		flags |= IsRightMousePressed() ? MOUSE_RIGHT : FLAG_NONE;
		flags |= IsMiddleMousePressed() ? MOUSE_MIDDLE : FLAG_NONE;

		return flags;
	}


	Event::Event(EventType type, View* sender)
		: type_(type)
		, sender_(sender)
	{

	}

	Event::Event(EventType type)
		: type_(type)
		, sender_(NULL)
	{

	}

	Event::~Event()
	{

	}

	void Event::StopPropagation()
	{
		is_propagation_ = false;
	}

	void Event::SetSender(View* v)
	{
		sender_ = v;
	}

	View* Event::sender() const
	{
		return sender_;
	}

}