#pragma once
#include "uikit/text/glyph_line.h"
#include "uikit/common/ui_constants.h"

namespace ui
{
	class UIGlyphPagraph
	{
	public:
		UIGlyphPagraph();
		~UIGlyphPagraph();

		void addGlyphFragment(UIGlyphFragment* fragment);

		void setLayoutWidth(int width);
		void setLineBreakMode(UILineBreakMode mode);

		void Render(UIRenderContext* context);
	private:
		friend class UIGlyphLayout;
		void Layout();
		void addGlyphLine(UIGlyphLine* line);
		void clearGlyphLine();
		UIGlyphPagraph* prevGlyphPagraph_{ NULL };
		UIGlyphPagraph* nextGlyphPagraph_{ NULL };

		UIGlyphLine* firstGlyphLine_{ NULL };
		UIGlyphLine* lastGlyphLine_{ NULL };

		void clearGlyphFragment();

		UIGlyphFragment* fisrtGlyphFragment_{ NULL };
		UIGlyphFragment* lastGlyphFragment_{ NULL };

		UILineBreakMode lineBreakMode_{ UILineBreakByWordWrapping };
		int layoutWidth_{ -1 };

		Size size_;
		bool needLayout_{ true };
	};
}