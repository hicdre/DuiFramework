#include "stdafx.h"
#include "style_rule.h"

namespace ui
{


	StyleRule::~StyleRule()
	{

	}

	void StyleRule::AddDeclaction(StyleProperty p, StyleValue* v)
	{
		scoped_refptr<StyleDeclaration> declaration(new StyleDeclaration(p, v));
		AddDeclaction(declaration.get());
	}

	void StyleRule::AddDeclaction(StyleDeclaration* declaration)
	{
		declarations_->Insert(declaration);
	}

	void StyleRule::AddSelector(StyleSelector* selector)
	{
		selectors_->Add(selector);
	}

	StyleRule::StyleRule()
		: selectors_(new StyleSelectorList)
		, declarations_(new StyleDeclarationList)
		, line_(0)
	{

	}

	StyleRule::StyleRule(StyleSelectorList* l, StyleDeclarationList* d)
		: selectors_(l), declarations_(d), line_(0)
	{

	}

	void StyleRule::SetLineNumber(uint32 num)
	{
		line_ = num;
	}

}