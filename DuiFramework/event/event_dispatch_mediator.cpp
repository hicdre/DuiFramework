#include "stdafx.h"
#include "event_dispatch_mediator.h"
#include "event_include.h"

namespace ui
{


	scoped_refptr<EventDispatchMediator> EventDispatchMediator::Create(Event* event)
	{
		return scoped_refptr<EventDispatchMediator>(new EventDispatchMediator(event));
	}

	bool EventDispatchMediator::DispatchEvent(EventDispatcher* dispatcher) const
	{
		return dispatcher->Dispatch();
	}

	EventDispatchMediator::EventDispatchMediator(Event* event)
		: event_(event)
	{

	}

}