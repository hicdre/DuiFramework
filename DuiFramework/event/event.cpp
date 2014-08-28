#include "stdafx.h"
#include "event.h"

#include "core/view.h"
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

	int GetMouseFlags() {
		int flags = 0;
		flags |= IsLeftMousePressed() ? MOUSE_LEFT : FLAG_NONE;
		flags |= IsRightMousePressed() ? MOUSE_RIGHT : FLAG_NONE;
		flags |= IsMiddleMousePressed() ? MOUSE_MIDDLE : FLAG_NONE;
		return flags;
	}

	int GetKeyFlags() {
		int flags = 0;
		flags |= IsAltPressed() ? KEY_ALT : FLAG_NONE;
		flags |= IsShiftPressed() ? KEY_SHIFT : FLAG_NONE;
		flags |= IsCtrlPressed() ? KEY_CONTROL : FLAG_NONE;
		return flags;
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

	Event::Event(EventType type)
		: type_(type)
	{

	}

	Event::Event()
		: type_(EVENT_UNKNOWN)
	{

	}

	Event::~Event()
	{

	}

	void Event::StopDispatch()
	{
		is_continue_dispatch_ = false;
	}


	bool Event::IsMouseEvent() const
	{
		return (type_ == EVENT_MOUSE_MOVE
			|| type_ == EVENT_MOUSE_DOWN
			|| type_ == EVENT_MOUSE_UP
			|| type_ == EVENT_MOUSE_ENTER
			|| type_ == EVENT_MOUSE_LEAVE
			|| type_ == EVENT_MOUSE_DBCLICK);
	}

	bool Event::IsKeyEvent() const
	{
		return (type_ == EVENT_KEY_PRESSED
			|| type_ == EVENT_KEY_RELEASED
			|| type_ == EVENT_CHAR);
	}

	EventListener::EventListener()
	{
	}

	EventListener::~EventListener()
	{
	}

	EventListener& EventListener::Listen(EventType type, const EventCallback& c, uint32 group /*= 0*/)
	{
		listen_map_[type].Insert(c, group);
		return *this;
	}

	EventListener& EventListener::UnListen(EventType type)
	{
		listen_map_.erase(type);
		return *this;
	}

	EventListener& EventListener::UnListen(EventType type, uint32 group)
	{
		listen_map_[type].Remove(group);
		return *this;
	}

	EventListener& EventListener::UnListen(uint32 group)
	{
		for (auto iter = listen_map_.begin(); iter != listen_map_.end(); iter++)
		{
			iter->second.Remove(group);
		}
		return *this;
	}

	void EventListener::HandleEvent(Event* e)
	{
		if (!listen_map_.count(e->type()))
			return;

		listen_map_[e->type()].Invoke(e);
	}

	void EventCallbackSet::Insert(EventCallback c, uint32 group /*= 0*/)
	{
		Data d;
		d.c = c;
		d.group = group;
		container_.push_back(d);
	}

	void EventCallbackSet::Remove(uint32 group)
	{
		for (auto iter = container_.begin(); iter != container_.end();)
		{
			if (iter->group == group)
			{
				iter = container_.erase(iter);
			}
			else
			{
				iter++;
			}
		}
	}

	void EventCallbackSet::Invoke(Event* evt)
	{
		for (auto iter = container_.begin(); iter != container_.end(); iter++)
		{
			iter->c(evt);
		}
	}


}