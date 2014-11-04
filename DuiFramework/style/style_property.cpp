#include "stdafx.h"
#include "style_property.h"
#include <unordered_map>

namespace ui
{
	const char* kStyleProperty[] = {
		"width",
		"height",
	};

	static std::unordered_map<std::string, StyleProperty> kStylePropertyMap;


	void InitStyleProperty()
	{
		for (int i = Style_BEGIN; i < Style_END; i++)
		{
			kStylePropertyMap.insert({ kStyleProperty[i], (StyleProperty)i });
		}
	}


	const char* StyleFindString(StyleProperty p)
	{
		return kStyleProperty[p];
	}

	ui::StyleProperty StyleFindProperty(const char* str)
	{
		return kStylePropertyMap[std::string(str)];
	}

	



}