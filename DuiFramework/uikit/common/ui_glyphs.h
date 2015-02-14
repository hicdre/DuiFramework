#pragma once
#include "foundation/foundation.h"

namespace ui
{
	typedef struct {
		uint32 index;
		double x_advance;
		double y_advance;
		double width;
		double height;
		double x_bearing;
		double y_bearing;
	} UIGlyph;
}