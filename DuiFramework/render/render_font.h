#pragma once
#include "render/render_forward.h"

namespace ui
{
	class RenderFont : public RefCounted < RenderFont >
	{
	public:
		virtual ~RenderFont() {}

		virtual void GetGlyphIndices(const RenderContext* context,
			const wchar_t* str, size_t len, uint32* glyphs) = 0;
	};
}