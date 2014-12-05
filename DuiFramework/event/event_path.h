#pragma once
#include "event/event_forward.h"
#include <deque>

namespace ui
{
	class UIElement;
	class EventPath
	{
	public:
		explicit EventPath();
		explicit EventPath(UIElement* elem);
		~EventPath();

		bool isEmpty() const { return elements_.empty(); }
		size_t size() const { return elements_.size(); }

		UIElement* at(size_t index) const { return elements_[index]; }
		UIElement* front() const;
		UIElement* back() const;

		void push_back(UIElement* elem) { elements_.push_back(elem); }
		void pop_front() { elements_.pop_front(); }
		void pop_back() { elements_.pop_back(); }

		void reverse();
	private:
		std::deque<UIElement*> elements_;
	};
}