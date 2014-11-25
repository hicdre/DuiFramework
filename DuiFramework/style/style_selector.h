#pragma once
#include "base/basictypes.h"
#include <set>

namespace ui
{
	class UIElement;
	enum PseudoType {
		PSEUDO_NULL = 0,

		PSEUDO_HOVER,
		PSEUDO_ACTIVE,
		PSEUDO_FOCUS,
	};
	//Ñ¡ÔñÆ÷
	// #id.tt.bb:hover
	// [id|tag]?[class]*[pseudo]
	class StyleSelector
	{
	public:
		StyleSelector();
		~StyleSelector();

		void Reset();

		bool HasId() const;
		bool HasTag() const;
		bool HasClass() const;
		bool IsPseudo() const;

		bool HasChild() const { return !!child_; }

		void SetId(const std::string& id);

		void SetTag(const std::string& tag);

		void SetPseduo(PseudoType type);

		void AddClass(const std::string& v);
		void RemoveClass(const std::string& v);

		void AddChildSelector(StyleSelector* s);

		bool MatchElement(UIElement* v) const;
	private:
		bool MatchElementInternal(UIElement* v) const;
		bool is_id_;
		std::string* id_or_tag_;
		std::set<std::string> class_list_;
		PseudoType type_;

		StyleSelector* child_;
	};

	class StyleSelectorList
	{
	public:
		StyleSelectorList();
		~StyleSelectorList();

		void Clear() {
			for (StyleSelector* item : container_)
			{
				delete item;
			}
			container_.clear();
		}

		int GetCount() const {
			return container_.size();
		}

		StyleSelector* Get(int index) const {
			return container_.at(index);
		}

		void Add(StyleSelector* s) {
			container_.push_back(s);
		}

		bool MatchElement(UIElement* v) const;
	private:
		std::vector<StyleSelector*> container_;
	};
}