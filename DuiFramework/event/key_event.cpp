#include "stdafx.h"
#include "key_event.h"

namespace ui
{


	KeyEvent::KeyEvent(EventType type, int key, View* v, uint8 count, uint8 flags)
		: Event(type, v)
		, key_(key)
		, count_(count)
		, flags_(flags)
	{

	}

	KeyEvent::KeyEvent(EventType type, int key, View* v, LPARAM lparam)
		: KeyEvent(type, key, v, HIWORD(lparam), LOWORD(lparam))
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

}