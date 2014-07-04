#include "stdafx.h"
#include "event.h"

#include "core/view.h"
#include "core/app.h"

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

	Point GetMouseLocation(const MSG& msg) {
		POINT pt;
		if (msg.message == WM_MOUSELEAVE ||
			msg.message == WM_NCMOUSELEAVE) 
		{
			// These events have no coordinates. For sanity with rest of events grab
			// coordinates from the OS.
			::GetCursorPos(&pt);
		}
		else if (IsClientMouseEvent(msg) &&
			!IsMouseWheelEvent(msg)) 
		{
			// Note: Wheel events are considered client, but their position is in screen
			//       coordinates.
			// Client message. The position is contained in the LPARAM.
			return Point(((int)(short)LOWORD(msg.lParam)), ((int)(short)HIWORD(msg.lParam)));
		}
		else 
		{
			// Non-client message. The position is contained in a POINTS structure in
			// LPARAM, and is in screen coordinates so we have to convert to client.
			pt.x = ((int)(short)LOWORD(msg.lParam));
			pt.y = ((int)(short)HIWORD(msg.lParam));
		}
		::ScreenToClient(msg.hwnd, &pt);
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

	int GetKeyFlags(const MSG& msg) {
		int flags = 0;
		flags |= IsAltPressed() ? KEY_ALT : FLAG_NONE;
		flags |= IsShiftPressed() ? KEY_SHIFT : FLAG_NONE;
		flags |= IsCtrlPressed() ? KEY_CONTROL : FLAG_NONE;

		// Most client mouse messages include key state information.
		if (IsClientMouseEvent(msg)) {
			int win_flags = GET_KEYSTATE_WPARAM(msg.wParam);
			flags |= (win_flags & MK_SHIFT) ? KEY_SHIFT : 0;
			flags |= (win_flags & MK_CONTROL) ? KEY_CONTROL : 0;
		}

		return flags;
	}

	// Returns a mask corresponding to the set of pressed mouse buttons.
	// This includes the button of the given message, even if it is being released.
	int GetMouseFlags(const MSG& msg) {
		int win_flags = GetMouseKey(msg);

		// Client mouse messages provide key states in their WPARAMs.
		if (IsClientMouseEvent(msg))
			win_flags |= GET_KEYSTATE_WPARAM(msg.wParam);

		int flags = 0;
		flags |= (win_flags & MK_LBUTTON) ? MOUSE_LEFT : 0;
		flags |= (win_flags & MK_MBUTTON) ? MOUSE_MIDDLE : 0;
		flags |= (win_flags & MK_RBUTTON) ? MOUSE_RIGHT : 0;
		return flags;
	}

	int GetEventFlags(const MSG& msg) {
		int flags = GetKeyFlags(msg);
		if (IsMouseEvent(msg))
			flags |= GetMouseFlags(msg);

		return flags;
	}

	Event::Event(EventType type, View* v)
		: type_(type)
		, owner_(v)
		, sender_(v)
	{

	}

	Event::~Event()
	{

	}

	void Event::StopPropagation()
	{
		is_propagation_ = false;
	}

	void Event::SetOwner(View* v)
	{
		owner_ = v;
	}

	View* Event::owner() const
	{
		return owner_;
	}

	void Event::SetSender(View* v)
	{
		sender_ = v;
	}

	View* Event::sender() const
	{
		return sender_;
	}


	MouseEvent::MouseEvent(EventType type, const Point& pt, View* owner, int flags)
		: Event(type, owner)
		, pt_(pt)
		, flags_(flags)
	{

	}

	int MouseEvent::flags() const
	{
		return flags_;
	}

	bool MouseEvent::IsAnyButton() const
	{
		return (flags() & (MOUSE_LEFT | MOUSE_MIDDLE |
			MOUSE_RIGHT)) != 0;
	}

	bool MouseEvent::IsRightMouseButton() const
	{
		return (flags() & MOUSE_RIGHT) != 0;
	}

	bool MouseEvent::IsOnlyRightMouseButton() const
	{
		return (flags() & MOUSE_RIGHT) &&
			!(flags() & (MOUSE_LEFT | MOUSE_MIDDLE));
	}

	bool MouseEvent::IsMiddleMouseButton() const
	{
		return (flags() & MOUSE_MIDDLE) != 0;
	}

	bool MouseEvent::IsOnlyMiddleMouseButton() const
	{
		return (flags() & MOUSE_MIDDLE) &&
			!(flags() & (MOUSE_LEFT | MOUSE_RIGHT));
	}

	bool MouseEvent::IsLeftMouseButton() const
	{
		return (flags() & MOUSE_LEFT) != 0;
	}

	bool MouseEvent::IsOnlyLeftMouseButton() const
	{
		return (flags() & MOUSE_LEFT) &&
			!(flags() & (MOUSE_MIDDLE | MOUSE_RIGHT));
	}

	Point MouseEvent::GetPosition() const
	{
		return pt_;
	}

	void MouseEvent::DispathTo(View* v)
	{
		View::ConvertPointToTarget(sender_, v, &pt_);
		sender_ = v;
	}


	EventDispatcher::EventDispatcher()
	{

	}

	EventDispatcher::~EventDispatcher()
	{

	}

	void EventDispatcher::Regist(View* v, EventListener* l)
	{
		listener_map_[v].insert(l);
	}

	void EventDispatcher::UnRegist(View* v, EventListener* l)
	{
		listener_map_[v].erase(l);
	}

	void EventDispatcher::UnRegistAll(View* v)
	{
		auto iter = listener_map_.find(v);
		if (iter == listener_map_.end())
			return;

		listener_map_.erase(iter);
	}

	void EventDispatcher::UnRegistListener(EventListener* l)
	{
		for (auto iter : listener_map_)
		{
			iter.second.erase(l);
		}
	}

	EventDispatcher* EventDispatcher::Default()
	{
		return App::Get()->DefaultEventDispatcher();
	}

	void EventDispatcher::DispatchMouseMove(View* v, MouseEvent& e)
	{
		auto iter = listener_map_.find(v);
		if (iter == listener_map_.end())
			return;

		const std::unordered_set<EventListener*> list = iter->second;
		for (EventListener* l : list)
		{
			l->OnMouseMove(v, e);
		}
	}

	


	EventListener::EventListener()
	{

	}

	EventListener::~EventListener()
	{
		EventDispatcher::Default()->UnRegistListener(this);
	}

}