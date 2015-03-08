#include "stdafx.h"
#include "text_pagraph.h"
#include "glyph_pagraph.h"

namespace ui
{

	TextPagraph::TextPagraph(const wchar_t* text, size_t begin, size_t end) 
		//: text_(text), begin_(begin), end_(end)
	{
		if (begin < end)
			addTextFragment(new TextFragment(text, begin, end));
	}

	TextPagraph::~TextPagraph()
	{
		clearTextFragment();
	}


// 	Rect TextPagraph::GetBoundsRect() const
// 	{
// 		return bounds_;
// 	}

// 	void TextPagraph::Layout(size_t width, UILineBreakMode lineBreakMode)
// 	{
// 		int remainWidth = width;
// 		size_t fragmentOffset = 0;
// 		UIGlyphLine* newLine = new UIGlyphLine;
// 		TextFragment* fragment = firstTextFragment_;
// 		while (fragment)
// 		{
// 			UIGlyphFragment* glyphFragment = glyphFragmentForWidth(fragment, remainWidth, fragmentOffset);
// 			if (glyphFragment)
// 				newLine->addGlyphFragment(glyphFragment);
// 			if (fragmentOffset >= fragment->glyphsCount()) // fragment用完，下一个
// 				fragment = fragment->next_;
// 			if (remainWidth == 0) { //width用完，下一行
// 				addGlyphLine(newLine);
// 				newLine = new UIGlyphLine;
// 			}
// 		}
// 		if (newLine->isEmpty())
// 			delete newLine;
// 		else
// 			addGlyphLine(newLine);
// 	}

// 	void TextPagraph::SetWidth(size_t width)
// 	{
// 		if (firstTextFragment_ == NULL)
// 			return;
// 
// 		if (bounds_.width() == width)
// 			return;
// 
// 		Layout(width, lineBreakMode_);
// 		CalcBoundsRect();
// 		if (isMutilLine())
// 			bounds_.set_width(width);
// 	}


// 	void TextPagraph::SetPosition(int x, int y)
// 	{
// 		bounds_.set_origin(Point(x,y));
// 	}

	void TextPagraph::addTextFragment(TextFragment* fragment)
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

	void TextPagraph::clearTextFragment()
	{
		TextFragment* a = firstTextFragment_;
		while (a)
		{
			TextFragment* b = a;
			a = a->next_;
			delete b;
		}
		firstTextFragment_ = lastTextFragment_ = NULL;
	}

	size_t TextPagraph::strLength() const
	{
		size_t length = 0;
		for (TextFragment* fragment = firstTextFragment_; fragment; fragment = fragment->next_)
		{
			length += fragment->strLength();
		}
		return length;
	}

	UIGlyphPagraph* TextPagraph::buildGlyphPagraph()
	{
		UIGlyphPagraph* pagraph = new UIGlyphPagraph(this);
		for (TextFragment* fragment = firstTextFragment_; fragment; fragment = fragment->nextTextFragment())
		{
			UIGlyphFragment* p = fragment->buildGlyphFragment();
			while (p) {
				UIGlyphFragment* n = p->nextFragment();
				pagraph->addGlyphFragment(p);
				p = n;
			}
			
		}
		return pagraph;
	}

	void TextPagraph::setTextAlignment(TextAlignment v)
	{
		if (textAlignment_ == v)
			return;
		textAlignment_ = v;
	}

	void TextPagraph::setLineBreakMode(UILineBreakMode v)
	{
		if (lineBreakMode_ == v)
			return;
		lineBreakMode_ = v;
	}

}