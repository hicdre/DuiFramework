#include "stdafx.h"
#include "event_dispatcher.h"
#include "event_include.h"

namespace ui
{


	bool EventDispatcher::DispatchEvent(UIElement* elem, EventDispatchMediator* mediator)
	{
		if (!mediator->event())
			return true;
		EventDispatcher dispatcher(elem, mediator->event());
		return mediator->DispatchEvent(&dispatcher);
	}

	bool EventDispatcher::Dispatch()
	{
		//todo:
		return !event_->defaultPrevented();
	}

	EventDispatcher::EventDispatcher(UIElement* elem, Event* event)
		: elem_(elem)
		, event_(event)
	{

	}

}