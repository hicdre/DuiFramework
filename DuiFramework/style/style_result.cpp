#include "stdafx.h"
#include "style_result.h"

namespace ui
{
	StyleResult::StyleResult(StyleSelector* s, StyleDeclarationList* d)
		: selector_(s)
	{
		for (scoped_refptr<StyleDeclaration> p : *d)
		{
			declarations_[p->GetProperty()] = p;
		}
	}

	StyleResult::~StyleResult()
	{
	}

	uint32 StyleResult::specificity() const
	{
		return selector_->specificity();
	}


	StyleValue* StyleResult::GetValue(StyleProperty p) const
	{
		if (declarations_.count(p))
			return declarations_.at(p)->GetValue();
		return NULL;
	}


	StyleResultList::StyleResultList(StyleResult* one)
		: first_(one)
	{

	}

	StyleResultList::StyleResultList()
		: first_(NULL)
	{

	}


	StyleResultList::~StyleResultList()
	{
		StyleResult* t = first_;
		while (t)
		{
			StyleResult* p = t;
			t = t->next_;
			delete p;
		}
	}

	bool StyleResultList::HaveResult()
	{
		return first_ != NULL;
	}

	void StyleResultList::AddResult(StyleResult* add)
	{
		if (first_ == NULL) {
			first_ = add;
			return;
		}

		StyleResult* current = first_;
		StyleResult* current_prev = NULL;
		while (current->specificity() > current->specificity())
		{
			current_prev = current;
			current = current->next_;
		}



		if (current_prev) {
			current_prev->next_ = add;
		}
		else {
			first_ = add;
		}

		add->next_ = current;
	}

	void StyleResultList::RemoveResult(StyleResult* item)
	{
		if (first_ == NULL) {
			return;
		}

		StyleResult* current = first_;
		StyleResult* current_prev = NULL;
		while (current != item)
		{
			current_prev = current;
			current = current->next_;
		}

		if (current_prev)
			current_prev->next_ = current->next_;
		else
			first_ = current->next_;

		delete item;
	}

	void StyleResultList::RemoveResult(StyleSelector* selector)
	{
		if (first_ == NULL) {
			return;
		}

		StyleResult* current = first_;
		StyleResult* current_prev = NULL;
		while (current->selector() != selector)
		{
			current_prev = current;
			current = current->next_;
		}

		if (current_prev)
			current_prev->next_ = current->next_;
		else
			first_ = current->next_;

		delete current;
	}

	StyleValue* StyleResultList::GetValue(StyleProperty p) const
	{
		StyleResult* t = first_;
		while (t)
		{
			StyleValue* r = t->GetValue(p);
			if (r)
				return r;
			t = t->next_;
		}
		return NULL;
	}

	void StyleResultList::Swap(StyleResultList* results)
	{
		StyleResult* p = first_;
		first_ = results->first_;
		results->first_ = p;
	}

}