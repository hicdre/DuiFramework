#include "stdafx.h"
#include "glyph_pagraph.h"
#include "text_pagraph.h"

namespace ui
{


	UIGlyphPagraph::UIGlyphPagraph(TextPagraph* textPagraph)
		: textPagraph_(textPagraph)
	{

	}

	UIGlyphPagraph::~UIGlyphPagraph()
	{
		clearGlyphFragment();
		clearGlyphLine();
	}

	void UIGlyphPagraph::addGlyphLine(UIGlyphLine* line)
	{
		if (lastGlyphLine_) {
			lastGlyphLine_->nextGlyphLine_ = line;
			line->prevGlyphLine_ = lastGlyphLine_;
			line->nextGlyphLine_ = NULL;
			lastGlyphLine_ = line;
		}
		else {
			assert(firstGlyphLine_ == NULL);
			firstGlyphLine_ = lastGlyphLine_ = line;
			line->prevGlyphLine_ = line->nextGlyphLine_ = NULL;
		}
		glyphLineCount_++;
	}

	void UIGlyphPagraph::clearGlyphLine()
	{
		UIGlyphLine* line = firstGlyphLine_;
		while (line)
		{
			UIGlyphLine* currentLine = line;
			line = line->nextGlyphLine_;
			delete currentLine;
		}
		firstGlyphLine_ = lastGlyphLine_ = NULL;
		glyphLineCount_ = 0;
	}

	void UIGlyphPagraph::Render(UIRenderContext* context)
	{
		int width = layoutWidth();
		UIScopedRender r(context);
		int prev_x = 0;
		for (UIGlyphLine* line = firstGlyphLine_; line; line = line->nextGlyphLine_)
		{
			Size size = line->lineSize();
			TextAlignment alignment = textPagraph_->textAlignment();
			int x = 0;
			if (alignment == TextAlignmentLeft)
				x = 0;
			else if (alignment == TextAlignmentCenter)
				x = width - size.width();
			else
				x = (width - size.width()) / 2;
			context->Translate(x - prev_x, 0);
			line->Render(context);
			context->Translate(0, size.height());
		}
	}

	void UIGlyphPagraph::setLayoutWidth(int width)
	{
		if (size_.width() == width)
			return;
		layoutWidth_ = width;
		needLayout_ = true;
	}



	void UIGlyphPagraph::Layout()
	{
		if (!needLayout_)
			return;
		
		clearGlyphLine();

		UIGlyphFragment* fragment = fisrtGlyphFragment_;
		UIGlyphLine* line = new UIGlyphLine;
		int remainWidth = layoutWidth_;
		while (fragment)
		{
			if (fragment->adjustForWidth(remainWidth, textPagraph_->lineBreakMode())) {
				line->addGlyphFragment(fragment);
				addGlyphLine(line);
				line = new UIGlyphLine;
			}
			else {
				remainWidth -= fragment->width();
				line->addGlyphFragment(fragment);
			}
			fragment = fragment->nextFragment();
		}

		needLayout_ = false;
	}

	void UIGlyphPagraph::clearGlyphFragment()
	{
		UIGlyphFragment* a = fisrtGlyphFragment_;
		while (a)
		{
			UIGlyphFragment* current = a;
			a = a->nextFragment();
			delete current;
		}
		fisrtGlyphFragment_ = lastGlyphFragment_ = NULL;
	}


	void UIGlyphPagraph::addGlyphFragment(UIGlyphFragment* fragment)
	{
		if (lastGlyphFragment_) {
			lastGlyphFragment_->nextFragment_ = fragment;
			fragment->prevFragment_ = lastGlyphFragment_;
			fragment->nextFragment_ = NULL;
			lastGlyphFragment_ = fragment;
		}
		else {
			assert(fisrtGlyphFragment_ == NULL);
			fisrtGlyphFragment_ = lastGlyphFragment_ = fragment;
			fragment->prevFragment_ = fragment->nextFragment_ = NULL;
		}
		needLayout_ = true;
	}

	int UIGlyphPagraph::layoutHeight()
	{
		Layout();
		int h = 0;
		for (UIGlyphLine* line = firstGlyphLine_; line; line = line->nextGlyphLine_)
		{
			h += line->lineHeight();
		}
		return h;
	}

	int UIGlyphPagraph::layoutWidth()
	{
		Layout();
		if (glyphLineCount_ == 1)
			return firstGlyphLine_->lineWidth();
		return layoutWidth_;
	}

	Size UIGlyphPagraph::layoutSize()
	{
		return Size(layoutWidth(), layoutHeight());
	}

}