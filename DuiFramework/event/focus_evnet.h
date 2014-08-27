#pragma once
#include "event/event.h"

namespace ui
{
	class FocusEvent : public Event
	{
	public:
		FocusEvent(EventType type, View* view_lose, View* gain);

		View* view_lose() const;
		View* view_gain() const;

	protected:
		View* view_lose_;
		View* view_gain_;
	};
}