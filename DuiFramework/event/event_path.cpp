#include "stdafx.h"
#include "event_path.h"
#include "event_include.h"

#include "dom/ui_include.h"

namespace ui
{
	EventPath::EventPath(Event* event)
		: event_(event)
		, elem_(NULL)
	{

	}

	EventPath::EventPath(UIElement* elem)
		: elem_(elem)
	{
		resetWith(elem);
	}


	void EventPath::resetWith(UIElement* elem)
	{
		elem_ = elem;
		elements_.clear();

		UIElement* current = elem_;
		elements_.push_back(current);
		while (current) {
			current = current->parent().get();
			if (current)
				elements_.push_back(current);
		}
	}

}