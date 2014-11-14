#include "stdafx.h"
#include "style_property.h"
#include <unordered_map>

namespace ui
{
#if 0
	// Flags for ParseVariant method
#define VARIANT_KEYWORD         0x000001  // K
#define VARIANT_LENGTH          0x000002  // L
#define VARIANT_PERCENT         0x000004  // P
#define VARIANT_COLOR           0x000008  // C eCSSUnit_Color, eCSSUnit_Ident (e.g.  "red")
#define VARIANT_URL             0x000010  // U
#define VARIANT_NUMBER          0x000020  // N
#define VARIANT_INTEGER         0x000040  // I
#define VARIANT_ANGLE           0x000080  // G
#define VARIANT_FREQUENCY       0x000100  // F
#define VARIANT_TIME            0x000200  // T
#define VARIANT_STRING          0x000400  // S
#define VARIANT_COUNTER         0x000800  //
#define VARIANT_ATTR            0x001000  //
#define VARIANT_IDENTIFIER      0x002000  // D
#define VARIANT_IDENTIFIER_NO_INHERIT 0x004000 // like above, but excluding
	// 'inherit' and 'initial'
#define VARIANT_AUTO            0x010000  // A
#define VARIANT_INHERIT         0x020000  // H eCSSUnit_Initial, eCSSUnit_Inherit
#define VARIANT_NONE            0x040000  // O
#define VARIANT_NORMAL          0x080000  // M
#define VARIANT_SYSFONT         0x100000  // eCSSUnit_System_Font
#define VARIANT_GRADIENT        0x200000  // eCSSUnit_Gradient
#define VARIANT_TIMING_FUNCTION 0x400000  // cubic-bezier() and steps()
#define VARIANT_ALL             0x800000  //
#define VARIANT_IMAGE_RECT    0x01000000  // eCSSUnit_Function
	// This is an extra bit that says that a VARIANT_ANGLE allows unitless zero:
#define VARIANT_ZERO_ANGLE    0x02000000  // unitless zero for angles
#define VARIANT_CALC          0x04000000  // eCSSUnit_Calc
#define VARIANT_ELEMENT       0x08000000  // eCSSUnit_Element
#define VARIANT_POSITIVE_DIMENSION 0x10000000 // Only lengths greater than 0.0
#define VARIANT_NONNEGATIVE_DIMENSION 0x20000000 // Only lengths greater than or equal to 0.0

	// Common combinations of variants
#define VARIANT_AL   (VARIANT_AUTO | VARIANT_LENGTH)
#define VARIANT_LP   (VARIANT_LENGTH | VARIANT_PERCENT)
#define VARIANT_LN   (VARIANT_LENGTH | VARIANT_NUMBER)
#define VARIANT_AH   (VARIANT_AUTO | VARIANT_INHERIT)
#define VARIANT_AHLP (VARIANT_AH | VARIANT_LP)
#define VARIANT_AHI  (VARIANT_AH | VARIANT_INTEGER)
#define VARIANT_AHK  (VARIANT_AH | VARIANT_KEYWORD)
#define VARIANT_AHKLP (VARIANT_AHLP | VARIANT_KEYWORD)
#define VARIANT_AHL  (VARIANT_AH | VARIANT_LENGTH)
#define VARIANT_AHKL (VARIANT_AHK | VARIANT_LENGTH)
#define VARIANT_HK   (VARIANT_INHERIT | VARIANT_KEYWORD)
#define VARIANT_HKF  (VARIANT_HK | VARIANT_FREQUENCY)
#define VARIANT_HKI  (VARIANT_HK | VARIANT_INTEGER)
#define VARIANT_HKL  (VARIANT_HK | VARIANT_LENGTH)
#define VARIANT_HKLP (VARIANT_HK | VARIANT_LP)
#define VARIANT_HKLPO (VARIANT_HKLP | VARIANT_NONE)
#define VARIANT_HL   (VARIANT_INHERIT | VARIANT_LENGTH)
#define VARIANT_HI   (VARIANT_INHERIT | VARIANT_INTEGER)
#define VARIANT_HLP  (VARIANT_HL | VARIANT_PERCENT)
#define VARIANT_HLPN (VARIANT_HLP | VARIANT_NUMBER)
#define VARIANT_HLPO (VARIANT_HLP | VARIANT_NONE)
#define VARIANT_HTP  (VARIANT_INHERIT | VARIANT_TIME | VARIANT_PERCENT)
#define VARIANT_HMK  (VARIANT_HK | VARIANT_NORMAL)
#define VARIANT_HC   (VARIANT_INHERIT | VARIANT_COLOR)
#define VARIANT_HCK  (VARIANT_HK | VARIANT_COLOR)
#define VARIANT_HUK  (VARIANT_HK | VARIANT_URL)
#define VARIANT_HUO  (VARIANT_INHERIT | VARIANT_URL | VARIANT_NONE)
#define VARIANT_AHUO (VARIANT_AUTO | VARIANT_HUO)
#define VARIANT_HPN  (VARIANT_INHERIT | VARIANT_PERCENT | VARIANT_NUMBER)
#define VARIANT_PN   (VARIANT_PERCENT | VARIANT_NUMBER)
#define VARIANT_ALPN (VARIANT_AL | VARIANT_PN)
#define VARIANT_HN   (VARIANT_INHERIT | VARIANT_NUMBER)
#define VARIANT_HON  (VARIANT_HN | VARIANT_NONE)
#define VARIANT_HOS  (VARIANT_INHERIT | VARIANT_NONE | VARIANT_STRING)
#define VARIANT_LPN  (VARIANT_LP | VARIANT_NUMBER)
#define VARIANT_UK   (VARIANT_URL | VARIANT_KEYWORD)
#define VARIANT_UO   (VARIANT_URL | VARIANT_NONE)
#define VARIANT_ANGLE_OR_ZERO (VARIANT_ANGLE | VARIANT_ZERO_ANGLE)
#define VARIANT_LPCALC (VARIANT_LENGTH | VARIANT_CALC | VARIANT_PERCENT)
#define VARIANT_LNCALC (VARIANT_LENGTH | VARIANT_CALC | VARIANT_NUMBER)
#define VARIANT_LPNCALC (VARIANT_LNCALC | VARIANT_PERCENT)
#define VARIANT_IMAGE (VARIANT_URL | VARIANT_NONE | VARIANT_GRADIENT | \
                       VARIANT_IMAGE_RECT | VARIANT_ELEMENT)
#endif
	const char* kStyleProperty[] = {
		"width",
		"height",

		"margin-left",
		"margin-top",
		"margin-right",
		"margin-bottom",

		"background-color",
		"background-image",

		"border-left-color",
		"border-left-width",
		"border-top-color",
		"border-top-width",
		"border-right-color",
		"border-right-width",
		"border-bottom-color",
		"border-bottom-width",

		"cursor",
	};

	enum FlagType{
		FlagVariant = 0,
		FlatValueProperty,
		FlagCount,
	};

	const uint32 kStyleVariant[][FlagCount] = {
			{ VARIANT_ALP, PROPERTY_PARSE_VALUE | PROPERTY_VALUE_NONNEGATIVE },
			{ VARIANT_ALP, PROPERTY_PARSE_VALUE | PROPERTY_VALUE_NONNEGATIVE },

			{ VARIANT_ALP, PROPERTY_PARSE_VALUE },
			{ VARIANT_ALP, PROPERTY_PARSE_VALUE },
			{ VARIANT_ALP, PROPERTY_PARSE_VALUE },
			{ VARIANT_ALP, PROPERTY_PARSE_VALUE },

			{ VARIANT_COLOR, PROPERTY_PARSE_VALUE },
			{ VARIANT_IMAGE, PROPERTY_PARSE_VALUE_LIST },

			{ VARIANT_COLOR, PROPERTY_PARSE_VALUE },
			{ VARIANT_AL, PROPERTY_PARSE_VALUE | PROPERTY_VALUE_NONNEGATIVE },
			{ VARIANT_COLOR, PROPERTY_PARSE_VALUE },
			{ VARIANT_AL, PROPERTY_PARSE_VALUE | PROPERTY_VALUE_NONNEGATIVE },
			{ VARIANT_COLOR, PROPERTY_PARSE_VALUE },
			{ VARIANT_AL, PROPERTY_PARSE_VALUE | PROPERTY_VALUE_NONNEGATIVE },
			{ VARIANT_COLOR, PROPERTY_PARSE_VALUE },
			{ VARIANT_AL, PROPERTY_PARSE_VALUE | PROPERTY_VALUE_NONNEGATIVE },

			{ 0, PROPERTY_PARSE_VALUE | PROPERTY_PARSE_FUNCTION },
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

	uint32 StyleFindVariant(StyleProperty p)
	{
		if (Style_BEGIN <= p && p < Style_END)
			return kStyleVariant[p][FlagVariant];
		return 0;
	}

	uint32 StyleFindParseType(StyleProperty p)
	{
		if (Style_BEGIN <= p && p < Style_END)
			return kStyleVariant[p][FlatValueProperty] & PROPERTY_PARSE_PROPERTY_MASK;
		return 0;
	}

	uint32 StyleFindValueRestrictions(StyleProperty p)
	{
		if (Style_BEGIN <= p && p < Style_END)
			return kStyleVariant[p][FlatValueProperty] & PROPERTY_VALUE_RESTRICTION_MASK;
		return 0;
	}

	



}