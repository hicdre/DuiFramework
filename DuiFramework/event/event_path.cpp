#include "stdafx.h"
#include "event_path.h"
#include "event_include.h"

#include "dom/ui_include.h"
#include <algorithm>

namespace ui
{
	EventPath::EventPath()
	{
	}

	EventPath::EventPath(UIElement* elem)
	{
		UIElement* e = elem;
		while (e)
		{
			elements_.push_back(e);
			e = e->parent().get();
		}
	}

	EventPath::~EventPath()
	{

	}

	void EventPath::reverse()
	{
		std::reverse(elements_.begin(), elements_.end());
	}

	UIElement* EventPath::front() const
	{
		if (isEmpty())
			return NULL;
		return elements_.front();
	}

	UIElement* EventPath::back() const
	{
		if (isEmpty())
			return NULL;
		return elements_.back();
	}

}