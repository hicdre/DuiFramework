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
	class StyleSelectorNode
	{
	public:
		StyleSelectorNode();
		~StyleSelectorNode();

		void Reset();

		bool HasId() const;
		bool HasTag() const;
		bool HasClass() const;
		bool IsPseudo() const;

		int GetClassCount() const { return class_list_.size(); }

		bool HasChild() const { return !!next_; }

		void SetId(const std::string& id);

		void SetTag(const std::string& tag);

		void SetPseduo(PseudoType type);

		void AddClass(const std::string& v);
		void RemoveClass(const std::string& v);

		bool MatchElement(UIElement* v) const;
	private:
		friend class StyleSelector;
		bool is_id_;
		std::string* id_or_tag_;
		std::set<std::string> class_list_;
		PseudoType type_;

		StyleSelectorNode* next_;
	};

	class StyleSelector
	{
	public:
		StyleSelector();
		~StyleSelector();

		void AddChildSelector(StyleSelectorNode* node);

		bool MatchElement(UIElement* v) const;

		uint32 specificity() const;
	private:
		//id : 10
		//class : 10
		//tag, pseudo : 10
		uint32 specificity_;
		StyleSelectorNode* first_;
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