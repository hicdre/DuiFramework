#pragma once
#include "event/event_forward.h"

namespace ui
{
	class UIElement;
	class EventPath
	{
	public:
		explicit EventPath(Event*);
		explicit EventPath(UIElement*);
		void resetWith(UIElement*);

		bool isEmpty() const { return elements_.empty(); }
		size_t size() const { return elements_.size(); }


	private:
		UIElement* at(size_t index) { return elements_[index]; }

		std::vector<UIElement*> elements_;
		UIElement* elem_;
		scoped_refptr<Event> event_;
	};
}