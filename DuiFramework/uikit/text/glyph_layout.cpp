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

	void UIGlyphLayout::setLayoutWidth(int width)
	{
		if (layoutWidth_ == width)
			return;
		layoutWidth_ = width;
		needLayout_ = true;
	}

	void UIGlyphLayout::Layout()
	{
		if (!needLayout_)
			return;
		for (UIGlyphPagraph* pagraph = firstPagraph_; pagraph; pagraph = pagraph->nextGlyphPagraph_)
		{
			pagraph->setLayoutWidth(layoutWidth_);
		}
		needLayout_ = false;
	}

	Size UIGlyphLayout::layoutSize()
	{
		return Size(layoutWidth(), layoutHeight());
	}

	int UIGlyphLayout::layoutHeight()
	{
		Layout();
		int h = 0;
		for (UIGlyphPagraph* pagraph = firstPagraph_; pagraph; pagraph = pagraph->nextGlyphPagraph_)
		{
			h += pagraph->layoutHeight();
		}
		return h;
	}

	int UIGlyphLayout::layoutWidth()
	{
		Layout();
		int w = 0;
		for (UIGlyphPagraph* pagraph = firstPagraph_; pagraph; pagraph = pagraph->nextGlyphPagraph_)
		{
			int width = pagraph->layoutWidth();
			if (width > w)
				w = width;
		}
		return w;
	}

	void UIGlyphLayout::Render(UIRenderContext* context)
	{
		Layout();
		UIScopedRender r(context);
		for (UIGlyphPagraph* pagraph = firstPagraph_; pagraph; pagraph = pagraph->nextGlyphPagraph_)
		{
			int height = pagraph->layoutHeight();
			pagraph->Render(context);
			context->Translate(0, height);
		}
	}

}