#include "stdafx.h"
#include "text_pagraph.h"

namespace ui
{
	static bool isSpace(wchar_t ch) {
		return ch == ' ' || ch == '\t';
	}

	static bool isEnd(wchar_t ch) {
		return ch == '\r' || ch == '\n';
	}

	static size_t nextWord(const wchar_t* buffer, size_t from, size_t end, bool& isSpaceWord, bool& isEndLine)
	{
		isSpaceWord = isSpace(buffer[from]);
		isEndLine = isEnd(buffer[from]);
		for (size_t pos = from + 1; pos < end; ++pos)
		{
			bool b = isSpace(buffer[pos]);
			bool be = isEnd(buffer[pos]);
			if (b != isSpaceWord || be != isEndLine)
			{
				return pos - from;
			}
		}
		return end - from;
	}

	UITextPagraph::UITextPagraph(const wchar_t* text, size_t begin, size_t end) 
		//: text_(text), begin_(begin), end_(end)
	{
		if (begin < end)
			addTextFragment(new UITextFragment(text, begin, end));
	}

	UITextPagraph::~UITextPagraph()
	{
		clearGlyphLine();
		clearTextFragment();
	}

	void UITextPagraph::addGlyphLine(UIGlyphLine* line)
	{
		if (lastLine_) {
			lastLine_->nextGlyphLine_ = line;
			line->prevGlyphLine_ = lastLine_;
			line->nextGlyphLine_ = NULL;
			lastLine_ = line;
		}
		else {
			assert(firstLine_ == NULL);
			firstLine_ = lastLine_ = line;
			line->prevGlyphLine_ = line->nextGlyphLine_ = NULL;
		}
		lineCount_++;
	}

	void UITextPagraph::clearGlyphLine()
	{
		UIGlyphLine* line = firstLine_;
		while (line)
		{
			UIGlyphLine* currentLine = line;
			line = line->nextGlyphLine_;
			delete currentLine;
		}
		firstLine_ = lastLine_ = NULL;
		lineCount_ = 0;
	}

	void UITextPagraph::Render(UIRenderContext* context)
	{
		UIScopedRender r(context);
		for (UIGlyphLine* line = firstLine_; line; line = line->nextGlyphLine_)
		{
			line->Render(context);
			context->Translate(0, line->lineHeight());
		}
	}

	Rect UITextPagraph::GetBoundsRect() const
	{
		return bounds_;
	}

	void UITextPagraph::Layout(size_t width, UILineBreakMode lineBreakMode)
	{
		int remainWidth = width;
		size_t fragmentOffset = 0;
		UIGlyphLine* newLine = new UIGlyphLine;
		UITextFragment* fragment = firstTextFragment_;
		while (fragment)
		{
			UIGlyphFragment* glyphFragment = glyphFragmentForWidth(fragment, remainWidth, fragmentOffset);
			if (glyphFragment)
				newLine->addGlyphFragment(glyphFragment);
			if (fragmentOffset >= fragment->glyphsCount()) // fragment用完，下一个
				fragment = fragment->next_;
			if (remainWidth == 0) { //width用完，下一行
				addGlyphLine(newLine);
				newLine = new UIGlyphLine;
			}
		}
		if (newLine->isEmpty())
			delete newLine;
		else
			addGlyphLine(newLine);
	}

	void UITextPagraph::SetWidth(size_t width)
	{
		if (firstTextFragment_ == NULL)
			return;

		if (bounds_.width() == width)
			return;

		Layout(width, lineBreakMode_);
		CalcBoundsRect();
		if (isMutilLine())
			bounds_.set_width(width);
	}

	void UITextPagraph::CalcBoundsRect()
	{
		int width = 0;
		int height = 0;
		for (UIGlyphLine* line = firstLine_; line; line = line->nextGlyphLine_)
		{
			Size sz(line->lineSize());
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

	void UITextPagraph::addTextFragment(UITextFragment* fragment)
	{
		if (lastTextFragment_) {
			lastTextFragment_->next_ = fragment;
			fragment->prev_ = lastTextFragment_;
			fragment->next_ = NULL;
			lastTextFragment_ = fragment;
		}
		else {
			assert(firstTextFragment_ == NULL);
			firstTextFragment_ = lastTextFragment_ = fragment;
			fragment->prev_ = fragment->next_ = NULL;
		}
	}

	void UITextPagraph::clearTextFragment()
	{
		UITextFragment* a = firstTextFragment_;
		while (a)
		{
			UITextFragment* b = a;
			a = a->next_;
			delete b;
		}
		firstTextFragment_ = lastTextFragment_ = NULL;
	}

	size_t UITextPagraph::strLength() const
	{
		size_t length = 0;
		for (UITextFragment* fragment = firstTextFragment_; fragment; fragment = fragment->next_)
		{
			length += fragment->strLength();
		}
		return length;
	}

	UIGlyphFragment* UITextPagraph::glyphFragmentForWidth(UITextFragment* textFragment, int& width, size_t& offset)
	{
		int w = 0;
		size_t i = offset;
		while (i < textFragment->glyphsCount())
		{
			if (lineBreakMode_ == UILineBreakByCharWrapping)
			{
				w += glyphs_[i].x_advance;
				if (w > width)
					break;
				++i;
			}
			else if (lineBreakMode_ == UILineBreakByWordWrapping)
			{
				bool isSpaceWord = false;
				bool isEndLine = false;
				size_t wordCount = nextWord(textFragment->str(), i, textFragment->glyphsCount_,
					isSpaceWord, isEndLine);
				
				size_t wordWidth = 0;
				for (size_t j = 0; j < wordCount; ++j)
				{
					wordWidth += textFragment->glyphs_[j].x_advance;
				}
				if (w + wordWidth > width) {
					if (isSpaceWord)
						i += wordCount;
					break;
				}
				i += wordCount;
				w += wordWidth;
			}
		}
		
		UIGlyphFragment* f = new UIGlyphFragment(textFragment, offset, offset + i);
		if (i != textFragment->glyphsCount_)
			width = 0;
		else
			width -= w;

		offset += i;
		return f;
	}

}