#pragma once
#include "event/event_forward.h"

namespace ui
{
	class UIElement;
	class EventDispatcher
	{
	public:
		static bool DispatchEvent(UIElement*, EventDispatchMediator* mediator);

		bool Dispatch();
		UIElement* element() const { return elem_.get(); }
		Event* event() const { return event_.get(); }
	private:
		EventDispatcher(UIElement*, Event*);

		scoped_refptr<UIElement> elem_;
		scoped_refptr<Event> event_;
	};
}