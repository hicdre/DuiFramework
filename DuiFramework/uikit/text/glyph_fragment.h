#pragma once
#include "uikit/common/ui_font.h"
#include "uikit/common/ui_glyphs.h"
#include "uikit/render/ui_render_context.h"
#include "uikit/text/text_fragment.h"

namespace ui
{
	class UIGlyphFragment
	{
	public:
		UIGlyphFragment(UITextFragment* text,
			size_t begin, size_t end);
		~UIGlyphFragment();

		size_t glyphsCount() const;

		int width() const;
		int height() const;

		UIGlyphFragment* nextFragment() const;
		UIGlyphFragment* prevFragment() const;

		void Render(UIRenderContext* context);
	private:
		friend class UITextPagraph;
		friend class UIGlyphLine;
		UITextFragment* textFragment_;
		size_t begin_;
		size_t end_;

		UIGlyphFragment* nextFragment_{ NULL };
		UIGlyphFragment* prevFragment_{ NULL };
	};
}