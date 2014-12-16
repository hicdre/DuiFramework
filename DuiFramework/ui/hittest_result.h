#pragma once
#include "base/basictypes.h"

namespace ui
{
	class UIElement;
	class HitTestResult
	{
	public:
		struct ResultNode {
			Point location;
			UIElement* element;
			ResultNode* next;
		};
		HitTestResult();
		~HitTestResult();

		UIElement* element() const;
		const Point& point() const;

		void Add(ResultNode* r);
		void Add(UIElement* e, const Point& p);
	private:
		ResultNode* first_;
	};
}