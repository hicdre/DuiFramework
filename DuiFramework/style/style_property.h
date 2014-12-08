#pragma once
#include "base/basictypes.h"
#include <vector>

namespace ui
{
	enum StyleProperty
	{
		Style_UNKNOWN = -1,
		Style_BEGIN = 0,

		Style_Width = Style_BEGIN,
		Style_Height,

		Style_MarginLeft,
		Style_MarginTop,
		Style_MarginRight,
		Style_MarginBottom,

		Style_BackgroundColor,
		Style_BackgroundImage,

		Style_BorderLeftColor,
		Style_BorderLeftWidth,
		Style_BorderTopColor,
		Style_BorderTopWidth,
		Style_BorderRightColor,
		Style_BorderRightWidth,
		Style_BorderBottomColor,
		Style_BorderBottomWidth,

		Style_BorderLeftTopRadius,
		Style_BorderRightTopRadius,
		Style_BorderRightBottomRadius,
		Style_BorderLeftBottomRadius,

		Style_Cursor,

		Style_END,
	};	

	//variant
	const uint32 VARIANT_KEWORD	  = 1 << 0;
	const uint32 VARIANT_LENGTH	  = 1 << 1;
	const uint32 VARIANT_PERCENT  = 1 << 2;
	const uint32 VARIANT_COLOR	  = 1 << 3;
	const uint32 VARIANT_URL      = 1 << 4;
	const uint32 VARIANT_NUMBER   = 1 << 5;
	const uint32 VARIANT_INTEGER  = 1 << 6;
	const uint32 VARIANT_ANGLE    = 1 << 7;
	const uint32 VARIANT_STRING   = 1 << 8;
	const uint32 VARIANT_AUTO     = 1 << 9;
	const uint32 VARIANT_GRADIENT = 1 << 10;
	const uint32 VARIANT_NONE     = 1 << 11;
	const uint32 VARIANT_ZERO_ANGLE = 1 << 12;

	const uint32 VARIANT_IMAGE = VARIANT_URL | VARIANT_NONE | VARIANT_GRADIENT;
		//VARIANT_IMAGE_RECT | VARIANT_ELEMENT

	const uint32 VARIANT_POSITIVE_DIMENSION = 0x10000000; // Only lengths greater than 0.0
	const uint32 VARIANT_NONNEGATIVE_DIMENSION = 0x20000000; // Only lengths greater than or equal to 0.0

	const uint32 VARIANT_AL = VARIANT_AUTO | VARIANT_LENGTH;
	const uint32 VARIANT_LP = VARIANT_LENGTH | VARIANT_PERCENT;
	const uint32 VARIANT_LN = VARIANT_LENGTH | VARIANT_NUMBER;
	const uint32 VARIANT_ALP = VARIANT_AL | VARIANT_PERCENT;

	//parse property flags
	const uint32 PROPERTY_PARSE_PROPERTY_MASK = 7 << 9;
	const uint32 PROPERTY_PARSE_FUNCTION = 2 << 9;
	const uint32 PROPERTY_PARSE_VALUE = 3 << 9;
	const uint32 PROPERTY_PARSE_VALUE_LIST = 4 << 9;
	
	//value restriction flags
	const uint32 PROPERTY_VALUE_RESTRICTION_MASK = 3 << 13;
	const uint32 PROPERTY_VALUE_NONNEGATIVE = 1 << 13;
	const uint32 PROPERTY_VALUE_AT_LEAST_ONE = 2 << 13;

	void InitStyleProperty();

	const char* StyleFindString(StyleProperty p);
	StyleProperty StyleFindProperty(const char* str);
	StyleProperty StyleFindProperty(const std::string& str);
	uint32 StyleFindVariant(StyleProperty p);
	uint32 StyleFindParseType(StyleProperty p);
	uint32 StyleFindValueRestrictions(StyleProperty p);

	typedef std::vector<StyleProperty> StylePropertyList;
}