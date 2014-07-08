#pragma once
#include "event/event.h"

namespace ui
{
	class KeyEvent : public Event
	{
	public:
		KeyEvent(EventType type, int key,
			int keyStates, unsigned short count, unsigned short flags);


	};
}