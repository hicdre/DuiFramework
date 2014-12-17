#pragma once
#include "base/basictypes.h"

namespace ui
{
	struct Glyph
	{
		uint32 index;
		Point pos;
	};

	struct GlyphBuffer
	{
		// A pointer to a buffer of glyphs. Managed by the caller.
		const Glyph *glyphs;
		// Number of glyphs mGlyphs points to.
		uint32 count;
	};
}