#pragma once
#include "base/basictypes.h"
#include <vector>

namespace ui
{
	enum StyleProperty
	{
		Style_BEGIN = 0,

		Style_Width = Style_BEGIN,
		Style_Height,

		Style_END,
	};	

	void InitStyleProperty();

	const char* StyleFindString(StyleProperty p);
	StyleProperty StyleFindProperty(const char* str);

	typedef std::vector<StyleProperty> StylePropertyList;
}