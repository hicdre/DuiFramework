#include "stdafx.h"
#include "input_event.h"
#include "event_include.h"

namespace ui
{
	InputEvent::InputEvent()
		: Event()
		, widget_(NULL)
		, modifiers_(0)
		, timestamps_(0)
	{

	}

	InputEvent::InputEvent(EventType type, bool canBubble, bool cancelable, Widget* widget)
		: Event(type, canBubble, cancelable)
		, widget_(widget)
		, modifiers_(0)
		, timestamps_(0)
	{

	}


	InputEvent::~InputEvent()
	{

	}


	

}