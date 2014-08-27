#include "stdafx.h"
#include "focus_evnet.h"

namespace ui
{


	FocusEvent::FocusEvent(EventType type, View* view_lose, View* view_gain)
		: Event(type), view_lose_(view_lose), view_gain_(view_gain)
	{

	}

	View* FocusEvent::view_lose() const
	{
		return view_lose_;
	}

	View* FocusEvent::view_gain() const
	{
		return view_gain_;
	}

}