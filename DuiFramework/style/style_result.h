#pragma once
#include "style/style_rule.h"

namespace ui
{
	class StyleResult
	{
	public:
		StyleResult(StyleSelector* s, StyleDeclarationList* d);
		~StyleResult();

		uint32 specificity() const;

		StyleSelector* selector() const { return selector_; }

		StyleValue* GetValue(StyleProperty p) const;
	private:
		friend class StyleResultList;
		StyleSelector* selector_;
		std::map<StyleProperty, scoped_refptr<StyleDeclaration>> declarations_;
		StyleResult* next_;

		DISALLOW_COPY_AND_ASSIGN(StyleResult);
	};

	class StyleResultList
	{
	public:
		StyleResultList();
		StyleResultList(StyleResult* one);
		~StyleResultList();

		bool HaveResult();
		void AddResult(StyleResult* add);
		void RemoveResult(StyleResult* item);
		void RemoveResult(StyleSelector* selector);

		StyleValue* GetValue(StyleProperty p) const;
		void Swap(StyleResultList* results);
	private:
		StyleResult* first_;
		DISALLOW_COPY_AND_ASSIGN(StyleResultList);
	};


}