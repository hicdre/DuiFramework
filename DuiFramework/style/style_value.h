#pragma once
#include "base/basictypes.h"

namespace ui
{
	enum StyleValueType
	{
		StyleValue_Null,
		StyleValue_None,
		StyleValue_Auto,

		StyleValue_Interger,
		StyleValue_Percent,
		StyleValue_Number,

		StyleValue_Color,

		StyleValue_Degree,

		StyleValue_Array,
		StyleValue_Pair,
		StyleValue_Triplet,
		StyleValue_Rect,

		StyleValue_Function,

	};
	class StyleValue
	{
	public:
		
		StyleValue(StyleValueType t);


	private:
		StyleValueType type_;
		union {
			int32 int_value_;
			uint32 uint_value;
			float float_value;

		};
	};
}