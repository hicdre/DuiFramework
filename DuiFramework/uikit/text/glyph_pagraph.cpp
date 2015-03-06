#include "stdafx.h"
#include "glyph_pagraph.h"

namespace ui
{


	UIGlyphPagraph::UIGlyphPagraph()
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
	}

	void UIGlyphPagraph::Render(UIRenderContext* context)
	{
		UIScopedRender r(context);
		for (UIGlyphLine* line = firstGlyphLine_; line; line = line->nextGlyphLine_)
		{
			line->Render(context);
			context->Translate(0, line->lineHeight());
		}
	}

	void UIGlyphPagraph::setLayoutWidth(int width)
	{
		if (size_.width() == width)
			return;
		layoutWidth_ = width;
		needLayout_ = true;
	}


	void UIGlyphPagraph::setLineBreakMode(UILineBreakMode mode)
	{
		if (lineBreakMode_ == mode)
			return;
		lineBreakMode_ = mode;
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
			if (fragment->adjustForWidth(remainWidth, lineBreakMode_)) {
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

}