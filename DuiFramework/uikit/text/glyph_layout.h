#pragma once
#include "uikit/text/glyph_pagraph.h"

namespace ui
{
	class UIGlyphLayout
	{
	public:
		UIGlyphLayout();
		~UIGlyphLayout();

		void addGlyphPagraph(UIGlyphPagraph* pagraph);
		void clearGlyphPagraph();

		void setLayoutWidth(int width);
		Size layoutSize();
		int layoutHeight();
		int layoutWidth();

		void Render(UIRenderContext* context);
	private:
		void Layout();

		UIGlyphPagraph* firstPagraph_{ NULL };
		UIGlyphPagraph* lastPagraph_{ NULL };
		size_t pagraphCount_{ 0 };

		int layoutWidth_{ 0 };
		bool needLayout_{ true };
	};
}