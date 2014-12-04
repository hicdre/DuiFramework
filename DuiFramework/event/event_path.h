#pragma once
#include "event/event_forward.h"

namespace ui
{
	class UIElement;
	class EventPath
	{
	public:
		explicit EventPath(UIElement*);
		void resetWith(UIElement*);

		bool isEmpty() const { return elements_.empty(); }
		size_t size() const { return elements_.size(); }

		UIElement* at(size_t index) { return elements_[index]; }

	private:
		std::vector<UIElement*> elements_;
		UIElement* elem_;
	};
}