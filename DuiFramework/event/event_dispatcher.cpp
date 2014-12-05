#include "stdafx.h"
#include "event_dispatcher.h"
#include "event_include.h"

namespace ui
{


	bool EventDispatcher::DispatchEvent(UIElement* elem, Event* e)
	{
		EventDispatcher dispatcher(elem, e);
		return dispatcher.Dispatch();
	}

	bool EventDispatcher::Dispatch()
	{
		event_->setTarget(elem_.get());

		DispatchEventAtBubbling();
		//todo:
		return !event_->defaultPrevented();
	}

	EventDispatcher::EventDispatcher(UIElement* elem, Event* event)
		: elem_(elem)
		, event_(event)
	{

	}

	void EventDispatcher::DispatchEventAtBubbling()
	{
		EventPath* event_path = event_->eventPath();
		size_t size = event_path->size();
		for (size_t i = 0; i < size; ++i) {
			UIElement* elem = event_path->at(i);
			if (elem == elem_.get())
				event_->setEventPhase(EVENT_AT_TARGET);
			else if (event_->bubbles() && !event_->cancelBubble())
				event_->setEventPhase(EVENT_BUBBLING_PHASE);
			else
				continue;
			event_->setCurrentTarget(elem);
			event_->AdjustForTarget();
			elem->HandleLocalEvents(event_.get());
			//eventContext.handleLocalEvents(m_event.get());
			if (event_->propagationStopped())
				return;
		}
		if (event_->bubbles() && !event_->cancelBubble()) {
			event_->setEventPhase(EVENT_BUBBLING_PHASE);
		}
	}

}