#include "stdafx.h"
#include "text_pagraph.h"

namespace ui
{


	UITextPagraph::UITextPagraph(const wchar_t* text, size_t begin, size_t end) : text_(text), begin_(begin), end_(end)
	{

	}

	UITextPagraph::~UITextPagraph()
	{
		destroy();
	}

	void UITextPagraph::addGlyphLine(UIGlyphLine* line)
	{
		if (lastLine_) {
			lastLine_->nextTextLine_ = line;
			line->prevTextLine_ = lastLine_;
			line->nextTextLine_ = NULL;
			lastLine_ = line;
		}
		else {
			assert(firstLine_ == NULL);
			firstLine_ = lastLine_ = line;
			line->prevTextLine_ = line->nextTextLine_ = NULL;
		}
		lineCount_++;
	}

	void UITextPagraph::destroy()
	{
		UIGlyphLine* line = firstLine_;
		while (line)
		{
			UIGlyphLine* currentLine = line;
			line = line->nextTextLine_;
			delete currentLine;
		}
		firstLine_ = lastLine_ = NULL;
		lineCount_ = 0;
	}

	void UITextPagraph::Render(UIRenderContext* context, Color color)
	{
		for (UIGlyphLine* line = firstLine_; line; line = line->nextTextLine_)
		{
			line->Render(context, color);
			context->Translate(0, line->calcTextHeight());
		}
	}

	Rect UITextPagraph::GetBoundsRect() const
	{
		return bounds_;
	}

	void UITextPagraph::Layout(const UIGlyph* glyphs, const wchar_t* str, size_t glyphCount, 
		UIRenderFont* font, size_t width, UILineBreakMode lineBreakMode)
	{
		const wchar_t* buffer = str;
		size_t advanceLen = glyphCount;
		while (advanceLen)
		{
			UIGlyphLine* textLine = new UIGlyphLine(glyphs, buffer, advanceLen, font);
			size_t glyphSize;
			if (lineBreakMode == UILineBreakByWordWrapping)
				glyphSize = textLine->layoutGlyphSizeForWidth(width);
			else if (lineBreakMode == UILineBreakByCharWrapping)
				glyphSize = textLine->layoutWordSizeForWidth(width);

			if (glyphSize < textLine->glyphsCount()) {
				textLine->setGlyphsCount(glyphSize);
				glyphs += glyphSize;
				buffer += glyphSize;
			}

			advanceLen -= glyphSize;
			addGlyphLine(textLine);
		}
	}

	void UITextPagraph::SetWidth(size_t width)
	{
		if (end_ <= begin_)
			return;

		if (bounds_.width() == width)
			return;

		scoped_refptr<UIRenderFont> renderFont(UIFont::CreateDefault()->renderFont());
		if (glyphs_ == NULL)
		{
			renderFont->GetGlyphs(text_ + begin_, end_ - begin_,
				&glyphs_, glyphsCount_);
		}
		Layout(glyphs_, text_ + begin_, glyphsCount_, renderFont, width, lineBreakMode_);
		CalcBoundsRect();
		if (isMutilLine())
			bounds_.set_width(width);
	}

	scoped_refptr<UIFont> UITextPagraph::GetFontAtIndex(size_t index)
	{
		return UIFont::CreateDefault();
	}

	void UITextPagraph::CalcBoundsRect()
	{
		size_t width = 0;
		size_t height = 0;
		for (UIGlyphLine* line = firstLine_; line; line = line->nextTextLine_)
		{
			Size sz(line->calcTextSize());
			if (sz.width() > width)
				width = sz.width();
			height += sz.height();
		}
		bounds_.SetSize(width, height);
	}

	void UITextPagraph::SetPosition(int x, int y)
	{
		bounds_.set_origin(Point(x,y));
	}

}