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
	private:

		UIGlyphPagraph* firstPagraph_{ NULL };
		UIGlyphPagraph* lastPagraph_{ NULL };
		size_t pagraphCount_{ 0 };
	};
}