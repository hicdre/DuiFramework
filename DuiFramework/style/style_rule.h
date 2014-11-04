#pragma once
#include "style/style_declaration.h"
#include "style/style_selector.h"

namespace ui
{
	class StyleRule
	{
	public:


		std::vector<StyleSelector*> selectors_;
		std::vector<StyleDeclaration*> declarations_;
	};
}