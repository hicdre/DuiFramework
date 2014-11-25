#include "stdafx.h"
#include "style_selector.h"
#include <stack>
#include "dom/dom_include.h"

namespace ui
{


	void StyleSelector::AddChildSelector(StyleSelector* s)
	{
		StyleSelector* v = this;
		while (v->child_)
		{
			v = v->child_;
		}
		v->child_ = s;
	}

	void StyleSelector::RemoveClass(const std::string& v)
	{
		class_list_.erase(v);
	}

	void StyleSelector::AddClass(const std::string& v)
	{
		class_list_.insert(v);
	}

	void StyleSelector::SetPseduo(PseudoType type)
	{
		type_ = type;
	}

	void StyleSelector::SetTag(const std::string& tag)
	{
		if (id_or_tag_)
			delete id_or_tag_;
		is_id_ = false;
		id_or_tag_ = new std::string(tag);
	}

	void StyleSelector::SetId(const std::string& id)
	{
		if (id_or_tag_)
			delete id_or_tag_;
		is_id_ = true;
		id_or_tag_ = new std::string(id);
	}

	bool StyleSelector::IsPseudo() const
	{
		return type_ != PSEUDO_NULL;
	}

	bool StyleSelector::HasTag() const
	{
		return id_or_tag_ && !is_id_;
	}

	bool StyleSelector::HasClass() const
	{
		return !class_list_.empty();
	}

	bool StyleSelector::HasId() const
	{
		return id_or_tag_ && is_id_;
	}

	void StyleSelector::Reset()
	{
		if (id_or_tag_) {
			delete id_or_tag_;
			id_or_tag_ = NULL;
		}
		class_list_.clear();
		type_ = PSEUDO_NULL;

		if (child_) {
			delete child_;
		}
	}

	StyleSelector::~StyleSelector()
	{
		Reset();
	}

	StyleSelector::StyleSelector() 
		: id_or_tag_(NULL), child_(NULL)
		, is_id_(false), type_(PSEUDO_NULL)
	{

	}

	bool StyleSelector::MatchElement(UIElement* v) const
	{
		std::stack<const StyleSelector*> s;
		{
			const StyleSelector* selector = this;
			s.push(selector);
			while (selector->child_)
			{
				s.push(selector);
				selector = selector->child_;
			}
		}

		const StyleSelector* selector = s.top();
		if (!selector->MatchElementInternal(v))
			return false;

		s.pop();
		v = dynamic_cast<UIElement*>(v->parent().get());
		while (!s.empty() && !v)
		{
			const StyleSelector* selector = s.top();
			if (selector->MatchElementInternal(v))
				s.pop();

			v = dynamic_cast<UIElement*>(v->parent().get());
		}

		return s.empty();
	}

	bool StyleSelector::MatchElementInternal(UIElement* v) const
	{
		if (HasId() && (v->getId() != *id_or_tag_))
			return false;

		if (HasTag() && (v->getTag() != *id_or_tag_))
			return false;

		if (HasClass()) {
			for (const std::string& c : class_list_)
			{
				if (!v->haveClass(c))
					return false;
			}
		}

		if (IsPseudo()) {
			//ÔÝÊ±²»¹Ü
			//if (type_ == PSEUDO_HOVER && v->Is) 
		}
		return true;
	}


	StyleSelectorList::StyleSelectorList()
	{

	}

	StyleSelectorList::~StyleSelectorList()
	{

	}

	bool StyleSelectorList::MatchElement(UIElement* v) const
	{
		for (StyleSelector* item : container_)
		{
			if (item->MatchElement(v))
				return true;
		}
		return false;
	}

}