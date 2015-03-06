#pragma once
#include "uikit/text/glyph_fragment.h"

namespace ui
{
	class UIGlyphPagraph;
	class UIGlyphLine
	{
	public:
		UIGlyphLine();
		~UIGlyphLine();

		bool isEmpty() const { return fisrtGlyphFragment_ == NULL; }
		int lineHeight() const;
		int lineWidth() const;
		Size lineSize() const;

		UIGlyphLine* nextGlyphLine() const;
		UIGlyphLine* prevGlyphLine() const;

		void clearGlyphFragment();
		void addGlyphFragment(UIGlyphFragment* fragment);

		void Render(UIRenderContext* context);
		
		void adjustForWidth(UIGlyphPagraph* pagraph, int width);
	private:
		friend class UIGlyphPagraph;

		UIGlyphLine* nextGlyphLine_{ NULL };
		UIGlyphLine* prevGlyphLine_{ NULL };

		UIGlyphFragment* fisrtGlyphFragment_{ NULL };
		UIGlyphFragment* lastGlyphFragment_{ NULL };
	};
}