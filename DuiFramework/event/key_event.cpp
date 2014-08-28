#include "stdafx.h"
#include "key_event.h"

namespace ui
{


	KeyEvent::KeyEvent(EventType type, int key, View* v, uint8 count, uint8 flags)
		: Event(type)
		, key_(key)
		, count_(count)
		, flags_(flags)
		, sender_(v)
	{

	}

	KeyEvent::KeyEvent(EventType type, int key, View* v, LPARAM lparam)
		: KeyEvent(type, key, v, LOWORD(lparam), HIWORD(lparam))
	{

	}

	int KeyEvent::GetKey() const
	{
		return key_;
	}

	int KeyEvent::GetKeyState() const
	{
		return key_states_;
	}

	uint8 KeyEvent::GetRepeatCounts() const
	{
		return count_;
	}

	uint8 KeyEvent::GetFlags() const
	{
		return flags_;
	}

	void KeyEvent::SetSender(View* v)
	{
		sender_ = v;
	}

	View* KeyEvent::sender() const
	{
		return sender_;
	}

}