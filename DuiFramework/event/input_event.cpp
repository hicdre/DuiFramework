#include "stdafx.h"
#include "input_event.h"
#include "event_include.h"

namespace ui
{
	InputEvent::InputEvent()
		: Event()
		, modifiers_(0)
		, timestamps_(0)
	{

	}

	InputEvent::InputEvent(EventType type, bool canBubble, bool cancelable, int modifiers)
		: Event(type, canBubble, cancelable)
		, modifiers_(modifiers)
		, timestamps_(0)
	{

	}


	InputEvent::~InputEvent()
	{

	}


	

}