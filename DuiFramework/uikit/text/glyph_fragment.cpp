#include "stdafx.h"
#include "glyph_fragment.h"
#include "uikit/render/ui_render_font.h"
#include "uikit/text/text_fragment.h"

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

	UIGlyphFragment::UIGlyphFragment(TextFragment* text,
		size_t offset, size_t count)
		: textFragment_(text)
		, glyphsOffset_(offset)
		, glyphsCount_(count)
	{
		assert(count);
	}

	UIGlyphFragment::~UIGlyphFragment()
	{
	}

	UIGlyphFragment* UIGlyphFragment::prevFragment() const
	{
		return prevFragment_;
	}

	UIGlyphFragment* UIGlyphFragment::nextFragment() const
	{
		return nextFragment_;
	}

	size_t UIGlyphFragment::glyphsCount() const
	{
		return glyphsCount_;
	}

	int UIGlyphFragment::width() const
	{
		int w = 0;
		for (size_t i = 0; i < glyphsCount_; ++i)
		{
			w += glyphs()[glyphsOffset_ + i].x_advance;
		}
		return w;
	}


	void UIGlyphFragment::Render(UIRenderContext* context)
	{
		if (textFragment_->backgroundColor() != Color_Transparent) {
			context->FillRect(Rect(width(), height()), textFragment_->backgroundColor());
		}
		context->ShowGlyphs(glyphs() + glyphsOffset_, glyphsCount_, textFragment_->font()->renderFont(), textFragment_->textColor());
	}

	int UIGlyphFragment::height() const
	{
		return textFragment_->font()->renderFont()->GetHeight();
	}

	bool UIGlyphFragment::adjustForWidth(int width, UILineBreakMode mode)
	{
		int w = 0;
		const UIGlyph* glyph = glyphs() + glyphsOffset_;
		for (size_t i = 0; i < glyphsCount_; ++i)
		{
			if (mode == UILineBreakByCharWrapping)
			{
				if (w + glyph[i].x_advance > width)
				{
					//分割
					splitAt(i);
					return true;
				}
			}
			else if (mode == UILineBreakByWordWrapping)
			{
				bool isSpaceWord = false;
				bool isEndLine = false;
				size_t wordCount = nextWord(textFragment_->str(), i, glyphsCount_,
					isSpaceWord, isEndLine);

				size_t wordWidth = 0;
				for (size_t j = 0; j < wordCount; ++j)
				{
					wordWidth += glyph[i + j].x_advance;
				}
				if (w + wordWidth > width) {
					if (isSpaceWord)
						i += wordCount;
					splitAt(i);
					return true;
				}
				i += wordCount;
				w += wordWidth;
			}
			
		}
		return false;
	}

	UIGlyphFragment* UIGlyphFragment::splitAt(size_t i)
	{
		assert(0 < i && i < glyphsCount_);
		UIGlyphFragment* fragment = new UIGlyphFragment(textFragment_, glyphsOffset_, glyphsCount_ - i);
		fragment->nextFragment_ = nextFragment_;
		fragment->prevFragment_ = this;
		nextFragment_ = fragment;
		glyphsCount_ = i;
		return this;
	}

	const UIGlyph* UIGlyphFragment::glyphs() const
	{
		return textFragment_->glyphs();
	}

}