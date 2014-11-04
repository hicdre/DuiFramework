#pragma once
#include "base/basictypes.h"

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
}