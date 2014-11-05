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
		if (Style_BEGIN <= p && p < Style_END)
			return kStyleProperty[p];
		return NULL;
	}

	ui::StyleProperty StyleFindProperty(const char* str)
	{
		if (str == NULL)
			return Style_UNKNOWN;
		
		return StyleFindProperty(std::string(str));
	}

	ui::StyleProperty StyleFindProperty(const std::string& str)
	{
		if (kStylePropertyMap.count(str))
			return kStylePropertyMap.at(str);
		return Style_UNKNOWN;
	}

	



}