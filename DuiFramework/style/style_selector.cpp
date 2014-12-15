#include "stdafx.h"
#include "style_selector.h"
#include <stack>
#include "dom/ui_include.h"

namespace ui
{

	void StyleSelectorNode::RemoveClass(const std::string& v)
	{
		class_list_.erase(v);
	}

	void StyleSelectorNode::AddClass(const std::string& v)
	{
		class_list_.insert(v);
	}

	void StyleSelectorNode::SetPseduo(PseudoType type)
	{
		type_ = type;
	}

	void StyleSelectorNode::SetTag(const std::string& tag)
	{
		if (id_or_tag_)
			delete id_or_tag_;
		is_id_ = false;
		id_or_tag_ = new std::string(tag);
	}

	void StyleSelectorNode::SetId(const std::string& id)
	{
		if (id_or_tag_)
			delete id_or_tag_;
		is_id_ = true;
		id_or_tag_ = new std::string(id);
	}

	bool StyleSelectorNode::IsPseudo() const
	{
		return type_ != PSEUDO_NULL;
	}

	bool StyleSelectorNode::HasTag() const
	{
		return id_or_tag_ && !is_id_;
	}

	bool StyleSelectorNode::HasClass() const
	{
		return !class_list_.empty();
	}

	bool StyleSelectorNode::HasId() const
	{
		return id_or_tag_ && is_id_;
	}

	void StyleSelectorNode::Reset()
	{
		if (id_or_tag_) {
			delete id_or_tag_;
			id_or_tag_ = NULL;
		}
		class_list_.clear();
		type_ = PSEUDO_NULL;

		if (next_) {
			delete next_;
			next_ = NULL;
		}
	}

	StyleSelectorNode::~StyleSelectorNode()
	{
		Reset();
	}

	StyleSelectorNode::StyleSelectorNode() 
		: id_or_tag_(NULL), next_(NULL)
		, is_id_(false), type_(PSEUDO_NULL)
	{

	}

	bool StyleSelectorNode::MatchElement(UIElement* v) const
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
			if (type_ == PSEUDO_HOVER && v->hovered())
				return true;
			if (type_ == PSEUDO_ACTIVE && v->actived())
				return true;
			if (type_ == PSEUDO_FOCUS && v->focused())
				return true;
			return false;
		}
		return true;
	}


	StyleSelectorList::StyleSelectorList()
	{

	}

	StyleSelectorList::~StyleSelectorList()
	{

	}

	bool StyleSelectorList::MatchElement(UIElement* v, StyleSelector** selector) const
	{
		for (StyleSelector* item : container_)
		{
			if (item->MatchElement(v)) {
				*selector = item;
				return true;
			}
		}
		return false;
	}


	StyleSelector::StyleSelector()
		: first_(NULL)
		, specificity_(0)
	{
		if (first_) {
			delete first_;
			first_ = NULL;
		}
	}

	StyleSelector::~StyleSelector()
	{

	}

	void StyleSelector::AddChildSelector(StyleSelectorNode* node)
	{
		node->next_ = first_;
		first_ = node;

		const uint32 mask = 0x07FF;
		uint32 id_count = specificity_ & (mask << 20);
		uint32 class_count = specificity_ & (mask << 10);
		uint32 tag_count = specificity_ & mask;
		if (node->HasId()) {
			id_count++;
		}
		if (node->HasTag()) {
			tag_count++;
		}
		if (node->IsPseudo()) {
			tag_count++;
		}
		class_count += node->GetClassCount();

		specificity_ = (id_count << 20) | (class_count << 10) | tag_count;
	}

	bool StyleSelector::MatchElement(UIElement* v) const
	{
		const StyleSelectorNode* selector = first_;
		if (!selector->MatchElement(v))
			return false;

		selector = first_->next_;
		v = v->parent().get();
		while (selector && v)
		{
			if (selector->MatchElement(v))
				selector = selector->next_;

			v = v->parent().get();
		}

		return selector == NULL;
	}

	uint32 StyleSelector::specificity() const
	{
		return specificity_;
	}

}