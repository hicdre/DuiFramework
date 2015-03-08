#pragma once
#include "uikit/text/glyph_line.h"
#include "uikit/common/ui_constants.h"

namespace ui
{
	class TextPagraph;
	class UIGlyphPagraph
	{
	public:
		UIGlyphPagraph(TextPagraph* textPagraph);
		~UIGlyphPagraph();

		void addGlyphFragment(UIGlyphFragment* fragment);

		void setLayoutWidth(int width);

		int layoutHeight();
		int layoutWidth();
		Size layoutSize();

		void Render(UIRenderContext* context);
	private:
		friend class UIGlyphLayout;
		void Layout();
		void addGlyphLine(UIGlyphLine* line);
		void clearGlyphLine();
		void clearGlyphFragment();
		UIGlyphPagraph* prevGlyphPagraph_{ NULL };
		UIGlyphPagraph* nextGlyphPagraph_{ NULL };

		UIGlyphLine* firstGlyphLine_{ NULL };
		UIGlyphLine* lastGlyphLine_{ NULL };
		int glyphLineCount_{ 0 };
		

		UIGlyphFragment* fisrtGlyphFragment_{ NULL };
		UIGlyphFragment* lastGlyphFragment_{ NULL };

		TextPagraph* textPagraph_;
		int layoutWidth_{ -1 };

		Size size_;
		bool needLayout_{ true };
	};
}