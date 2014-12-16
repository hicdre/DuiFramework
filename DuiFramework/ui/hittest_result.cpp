#include "stdafx.h"
#include "hittest_result.h"

namespace ui
{

	HitTestResult::HitTestResult()
		: first_(NULL)
	{

	}

	HitTestResult::~HitTestResult()
	{
		ResultNode* node = first_;
		while (node) {
			ResultNode* p = node->next;
			delete node;
			node = p;
		}
	}

	UIElement* HitTestResult::element() const
	{
		if (first_)
			return first_->element;
		return NULL;
	}

	const Point& HitTestResult::point() const
	{
		assert(first_);
		return first_->location;
	}

	void HitTestResult::Add(ResultNode* r)
	{
		r->next = first_;
		first_ = r;
	}

	void HitTestResult::Add(UIElement* e, const Point& p)
	{
		
		Add(new ResultNode{ p, e, NULL });
	}


}