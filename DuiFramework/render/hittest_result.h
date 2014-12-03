#pragma once
#include "dom/dom_element.h"

namespace ui
{
	class HittestResult
	{
	public:
		struct ResultNode {
			Point location;
			UIElement* element;
			ResultNode* next;
		};
		HittestResult();
		~HittestResult();

		UIElement* element() const;
		const Point& point() const;

		void Add(ResultNode* r);
		void Add(UIElement* e, const Point& p);
	private:
		ResultNode* first_;
	};
}