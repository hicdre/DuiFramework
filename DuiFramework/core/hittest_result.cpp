#include "stdafx.h"
#include "hittest_result.h"

namespace ui
{

	HittestResult::HittestResult()
		: first_(NULL)
	{

	}

	HittestResult::~HittestResult()
	{
		ResultNode* node = first_;
		while (node) {
			ResultNode* p = node->next;
			delete node;
			node = p;
		}
	}

	UIElement* HittestResult::element() const
	{
		if (first_)
			return first_->element;
		return NULL;
	}

	const Point& HittestResult::point() const
	{
		assert(first_);
		return first_->location;
	}

	void HittestResult::Add(ResultNode* r)
	{
		r->next = first_;
		first_ = r;
	}

	void HittestResult::Add(UIElement* e, const Point& p)
	{
		
		Add(new ResultNode{ p, e, NULL });
	}


}