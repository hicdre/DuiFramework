#include "stdafx.h"
#include "input_event_builder.h"

namespace ui
{
	static bool IsKeyDown(WPARAM wparam) {
		return (GetKeyState(wparam) & 0x8000) != 0;
	}

	static int GetLocationModifier(WPARAM wparam, LPARAM lparam) {
		int modifier = 0;
		switch (wparam) {
		case VK_RETURN:
			if ((lparam >> 16) & KF_EXTENDED)
				modifier = InputEvent::IsKeyPad;
			break;
		case VK_INSERT:
		case VK_DELETE:
		case VK_HOME:
		case VK_END:
		case VK_PRIOR:
		case VK_NEXT:
		case VK_UP:
		case VK_DOWN:
		case VK_LEFT:
		case VK_RIGHT:
			if (!((lparam >> 16) & KF_EXTENDED))
				modifier = InputEvent::IsKeyPad;
			break;
		case VK_NUMLOCK:
		case VK_NUMPAD0:
		case VK_NUMPAD1:
		case VK_NUMPAD2:
		case VK_NUMPAD3:
		case VK_NUMPAD4:
		case VK_NUMPAD5:
		case VK_NUMPAD6:
		case VK_NUMPAD7:
		case VK_NUMPAD8:
		case VK_NUMPAD9:
		case VK_DIVIDE:
		case VK_MULTIPLY:
		case VK_SUBTRACT:
		case VK_ADD:
		case VK_DECIMAL:
		case VK_CLEAR:
			modifier = InputEvent::IsKeyPad;
			break;
		case VK_SHIFT:
			if (IsKeyDown(VK_LSHIFT))
				modifier = InputEvent::IsLeft;
			else if (IsKeyDown(VK_RSHIFT))
				modifier = InputEvent::IsRight;
			break;
		case VK_CONTROL:
			if (IsKeyDown(VK_LCONTROL))
				modifier = InputEvent::IsLeft;
			else if (IsKeyDown(VK_RCONTROL))
				modifier = InputEvent::IsRight;
			break;
		case VK_MENU:
			if (IsKeyDown(VK_LMENU))
				modifier = InputEvent::IsLeft;
			else if (IsKeyDown(VK_RMENU))
				modifier = InputEvent::IsRight;
			break;
		case VK_LWIN:
			modifier = InputEvent::IsLeft;
			break;
		case VK_RWIN:
			modifier = InputEvent::IsRight;
			break;
		}

		assert(!modifier
			|| modifier == InputEvent::IsKeyPad
			|| modifier == InputEvent::IsLeft
			|| modifier == InputEvent::IsRight);
		return modifier;
	}

	static void SetToggleKeyState(InputEvent* event) {
		// Low bit set from GetKeyState indicates "toggled".
		int modifiers = event->modifiers();
		if (::GetKeyState(VK_NUMLOCK) & 1)
			modifiers |= InputEvent::NumLockOn;
		if (::GetKeyState(VK_CAPITAL) & 1)
			modifiers |= InputEvent::CapsLockOn;
		event->setModifiers(modifiers);
	}

	static int g_last_click_count = 0;
	static double g_last_click_time = 0;

	static LPARAM GetRelativeCursorPos(HWND hwnd) {
		POINT pos = { -1, -1 };
		GetCursorPos(&pos);
		ScreenToClient(hwnd, &pos);
		return MAKELPARAM(pos.x, pos.y);
	}

	scoped_refptr<MouseEvent> BuildMouseEvent(HWND hwnd,
		UINT message, WPARAM wparam, LPARAM lparam, 
		DWORD time_ms)
	{
		scoped_refptr<MouseEvent> result(new MouseEvent);

		switch (message) {
		case WM_MOUSEMOVE:
			result->setType(EVENT_MOUSE_MOVE);
			if (wparam & MK_LBUTTON)
				result->setButton(MouseEvent::ButtonLeft);
			else if (wparam & MK_MBUTTON)
				result->setButton(MouseEvent::ButtonMiddle);
			else if (wparam & MK_RBUTTON)
				result->setButton(MouseEvent::ButtonRight);
			else
				result->setButton(MouseEvent::ButtonNone);
			break;
		case WM_MOUSELEAVE:
			result->setType(EVENT_MOUSE_LEAVE);
			result->setButton(MouseEvent::ButtonNone);
			// set the current mouse position (relative to the client area of the
			// current window) since none is specified for this event
			lparam = GetRelativeCursorPos(hwnd);
			break;
		case WM_LBUTTONDOWN:
		case WM_LBUTTONDBLCLK:
			result->setType(EVENT_MOUSE_DOWN);
			result->setButton(MouseEvent::ButtonLeft);
			break;
		case WM_MBUTTONDOWN:
		case WM_MBUTTONDBLCLK:
			result->setType(EVENT_MOUSE_DOWN);
			result->setButton(MouseEvent::ButtonMiddle);
			break;
		case WM_RBUTTONDOWN:
		case WM_RBUTTONDBLCLK:
			result->setType(EVENT_MOUSE_DOWN);
			result->setButton(MouseEvent::ButtonRight);
			break;
		case WM_LBUTTONUP:
			result->setType(EVENT_MOUSE_UP);
			result->setButton(MouseEvent::ButtonLeft);
			break;
		case WM_MBUTTONUP:
			result->setType(EVENT_MOUSE_UP);
			result->setButton(MouseEvent::ButtonMiddle);
			break;
		case WM_RBUTTONUP:
			result->setType(EVENT_MOUSE_UP);
			result->setButton(MouseEvent::ButtonRight);
			break;
		default:
			assert(0);
		}

		assert(time_ms);
		result->setTimestamps(time_ms / 1000.0);

		// set position fields:

		int x = static_cast<short>(LOWORD(lparam));
		int y = static_cast<short>(HIWORD(lparam));
		result->setLocation(x, y);
		result->setClientLocation(x, y);

		POINT global_point = { x, y };
		ClientToScreen(hwnd, &global_point);

		result->setScreenLocation(global_point.x, global_point.y);

		// calculate number of clicks:

		// This differs slightly from the WebKit code in WebKit/win/WebView.cpp
		// where their original code looks buggy.
		static int last_click_position_x;
		static int last_click_position_y;
		static MouseEvent::Button last_click_button = MouseEvent::ButtonLeft;

		double current_time = result->timestamps();
		bool cancel_previous_click =
			(abs(last_click_position_x - result->x()) >
			(::GetSystemMetrics(SM_CXDOUBLECLK) / 2))
			|| (abs(last_click_position_y - result->y()) >
			(::GetSystemMetrics(SM_CYDOUBLECLK) / 2))
			|| ((current_time - g_last_click_time) * 1000.0 > ::GetDoubleClickTime());

		if (result->type() == EVENT_MOUSE_DOWN) {
			if (!cancel_previous_click && (result->button() == last_click_button)) {
				++g_last_click_count;
			}
			else {
				g_last_click_count = 1;
				last_click_position_x = result->x();
				last_click_position_y = result->y();
			}
			g_last_click_time = current_time;
			last_click_button = result->button();
		}
		else if (result->type() == EVENT_MOUSE_MOVE
			|| result->type() == EVENT_MOUSE_LEAVE) {
			if (cancel_previous_click) {
				g_last_click_count = 0;
				last_click_position_x = 0;
				last_click_position_y = 0;
				g_last_click_time = 0;
			}
		}
		result->setClickCount(g_last_click_count);

		// set modifiers:
		int modifiers = 0;
		if (wparam & MK_CONTROL)
			modifiers |= InputEvent::ControlKey;
		if (wparam & MK_SHIFT)
			modifiers |= InputEvent::ShiftKey;
		if (::GetKeyState(VK_MENU) & 0x8000)
			modifiers |= InputEvent::AltKey;
		if (wparam & MK_LBUTTON)
			modifiers |= InputEvent::LeftButtonDown;
		if (wparam & MK_MBUTTON)
			modifiers |= InputEvent::MiddleButtonDown;
		if (wparam & MK_RBUTTON)
			modifiers |= InputEvent::RightButtonDown;
		result->setModifiers(modifiers);

		SetToggleKeyState(&result);
		return result;
	}

}