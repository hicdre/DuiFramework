#pragma once
#include "style/style_declaration.h"
#include "style/style_selector.h"

namespace ui
{
	class StyleRule : public RefCounted<StyleRule>
	{
	public:
		StyleRule();

		void AddSelector(StyleSelector* selector);

		void AddDeclaction(StyleDeclaration* declaration);

		void AddDeclaction(StyleProperty p, StyleValue* v);

	private:
		friend class RefCounted < StyleRule > ;
		~StyleRule();
		StyleSelectorList selectors_;
		StyleDeclarationList declarations_;
		DISALLOW_COPY_AND_ASSIGN(StyleRule);
	};
}