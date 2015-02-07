#include "stdafx.h"
#include "ui_event_manager.h"
#include "uikit/widget/ui_window.h"

namespace ui
{

	static Point GetRelativeCursorPos(UIWindow* window) {
		POINT pos = { -1, -1 };
		GetCursorPos(&pos);
		return window->ConvertFromScreen(Point(pos.x, pos.y));
	}

	UIEventManager::UIEventManager()
		: mouse_(new UIMouse)
	{

	}

	UIEventManager::~UIEventManager()
	{

	}

	UIEventManager* UIEventManager::Get()
	{
		static UIEventManager instance;
		return &instance;
	}

	void UIEventManager::HanleMessage(UIWindow* window, UINT message, WPARAM w_param, LPARAM l_param)
	{
		if ((message >= WM_MOUSEFIRST && message <= WM_MOUSELAST)
			|| (message >= WM_NCMOUSEMOVE && message <= WM_NCXBUTTONDBLCLK)
			|| message == WM_MOUSELEAVE
			|| message == WM_NCMOUSELEAVE)
		{
			HandleMouseEvent(window, message, w_param, l_param);
		}
	}

	void UIEventManager::HandleMouseEvent(UIWindow* window, UINT message, WPARAM w_param, LPARAM l_param)
	{
		UIEvent* event = BuildMouseEvent(window, message, w_param, l_param);
		UIView* view = window->HitTestWithEvent(mouse_->positionInWindow(), event);
		mouse_->setView(view);
		if (view)
		{
			for (UIResponder* responder = view; responder; responder = responder->NextResponder())
			{
				switch (event->subType())
				{
				case EVENT_MOUSE_MOVE:
					responder->mouseMove(mouse_.get(), event);
					break;
				case EVENT_MOUSE_DOWN:
					responder->mousePress(mouse_.get(), event);
					break;
				case EVENT_MOUSE_UP:
					responder->mouseRelease(mouse_.get(), event);
					break;
				default:
					break;
				}
			}
			window->SetCursor(mouse_->cursor());
		}
		
	}

	UIEvent* UIEventManager::BuildMouseEvent(UIWindow* window,
		UINT message, 
		WPARAM wparam, 
		LPARAM lparam)
	{
		int x = static_cast<short>(LOWORD(lparam));
		int y = static_cast<short>(HIWORD(lparam));
		Point currentPosition(x, y);

		scoped_ptr<UIEvent> event(new UIEvent(EVENT_MOUSE));
		switch (message) {
		case WM_MOUSEMOVE:
			event->setSubType(EVENT_MOUSE_MOVE);
			if (wparam & MK_LBUTTON)
				mouse_->pressButton(UIMouse::ButtonLeft);
			else if (wparam & MK_MBUTTON)
				mouse_->pressButton(UIMouse::ButtonMiddle);
			else if (wparam & MK_RBUTTON)
				mouse_->pressButton(UIMouse::ButtonRight);
			else
				mouse_->pressButton(UIMouse::ButtonNone);
			break;
		case WM_MOUSELEAVE:
			event->setSubType(EVENT_MOUSE_MOVE);
			mouse_->pressButton(UIMouse::ButtonNone);
			// set the current mouse position (relative to the client area of the
			// current window) since none is specified for this event
			currentPosition = GetRelativeCursorPos(window);
			break;
		case WM_LBUTTONDOWN:
		case WM_LBUTTONDBLCLK:
			event->setSubType(EVENT_MOUSE_DOWN);
			mouse_->pressButton(UIMouse::ButtonLeft);
			break;
		case WM_MBUTTONDOWN:
		case WM_MBUTTONDBLCLK:
			event->setSubType(EVENT_MOUSE_DOWN);
			mouse_->pressButton(UIMouse::ButtonMiddle);
			break;
		case WM_RBUTTONDOWN:
		case WM_RBUTTONDBLCLK:
			event->setSubType(EVENT_MOUSE_DOWN);
			mouse_->pressButton(UIMouse::ButtonRight);
			break;
		case WM_LBUTTONUP:
			event->setSubType(EVENT_MOUSE_UP);
			mouse_->releaseButton(UIMouse::ButtonLeft);
			break;
		case WM_MBUTTONUP:
			event->setSubType(EVENT_MOUSE_UP);
			mouse_->releaseButton(UIMouse::ButtonMiddle);
			break;
		case WM_RBUTTONUP:
			event->setSubType(EVENT_MOUSE_UP);
			mouse_->releaseButton(UIMouse::ButtonRight);
			break;
		default:
			assert(0);
		}
		event->setTimestamps(GetTickCount());//ms
		mouse_->SetPositionInWindow(currentPosition);
		return event.release();
	}

	UIMouse* UIEventManager::mouse()
	{
		return mouse_.get();
	}

}