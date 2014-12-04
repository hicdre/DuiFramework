#pragma once
#include "event/event_forward.h"
#include "dom/ui_element.h"

namespace ui
{
	class EventDispatcher
	{
	public:
		static bool DispatchEvent(UIElement*, Event* e);

		bool Dispatch();
		UIElement* element() const { return elem_.get(); }
		Event* event() const { return event_.get(); }
	private:
		EventDispatcher(UIElement*, Event*);

		void DispatchEventAtBubbling();

		UIElementPtr elem_;
		scoped_refptr<Event> event_;
	};
}