#include "stdafx.h"
#include "style_selector.h"

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


	StyleSelectorList::StyleSelectorList()
	{

	}

	StyleSelectorList::~StyleSelectorList()
	{

	}

}