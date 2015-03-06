#include "stdafx.h"
#include "glyph_layout.h"

namespace ui
{


	UIGlyphLayout::UIGlyphLayout()
	{

	}

	UIGlyphLayout::~UIGlyphLayout()
	{

	}

	void UIGlyphLayout::addGlyphPagraph(UIGlyphPagraph* pagraph)
	{
		if (pagraph == NULL)
			return;
		if (lastPagraph_) {
			lastPagraph_->nextGlyphPagraph_ = pagraph;
			pagraph->prevGlyphPagraph_ = lastPagraph_;
			pagraph->nextGlyphPagraph_ = NULL;
			lastPagraph_ = pagraph;
		}
		else {
			assert(firstPagraph_ == NULL);
			firstPagraph_ = lastPagraph_ = pagraph;
			pagraph->prevGlyphPagraph_ = pagraph->nextGlyphPagraph_ = NULL;
		}
		pagraphCount_++;
	}

	void UIGlyphLayout::clearGlyphPagraph()
	{
		UIGlyphPagraph* pagraph = firstPagraph_;
		while (pagraph)
		{
			UIGlyphPagraph* deletePagraph = pagraph;
			pagraph = pagraph->nextGlyphPagraph_;
			delete deletePagraph;
		}
		firstPagraph_ = lastPagraph_ = NULL;
		pagraphCount_ = 0;
	}

}