#pragma once
#include "foundation/foundation.h"

namespace ui
{
	struct UIGlyph
	{
		uint32 index;
		Point pos;
	};

	struct UIGlyphBuffer
	{
		// A pointer to a buffer of glyphs. Managed by the caller.
		const UIGlyph *glyphs;
		// Number of glyphs mGlyphs points to.
		uint32 count;
	};
}