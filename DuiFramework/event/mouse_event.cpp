#include "stdafx.h"
#include "mouse_event.h"
#include "event_include.h"

namespace ui
{
	MouseEvent::MouseEvent()
		: m_button(ButtonNone)
		, m_clickCount(0)
	{

	}

	int MouseEvent::x() const
	{
		return m_clientLocation.x();
	}

	int MouseEvent::y() const
	{
		return m_clientLocation.y();
	}

	bool MouseEvent::IsMouseEvent() const
	{
		return true;
	}

	bool MouseEvent::IsDragEvent() const
	{
		return false;
	}

}