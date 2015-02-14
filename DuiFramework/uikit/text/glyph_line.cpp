#include "stdafx.h"
#include "glyph_line.h"

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

	UIGlyphLine::UIGlyphLine(const UIGlyph* glyphs, const wchar_t* buffer, size_t size, UIRenderFont* font) : glyphs_(glyphs)
		, glyphsCount_(size)
		, font_(font)
		, buffer_(buffer)
	{

	}

	UIGlyphLine::~UIGlyphLine()
	{

	}

	UIGlyphLine* UIGlyphLine::prevTextLine() const
	{
		return prevTextLine_;
	}

	UIGlyphLine* UIGlyphLine::nextTextLine() const
	{
		return nextTextLine_;
	}

	size_t UIGlyphLine::glyphsCount() const
	{
		return glyphsCount_;
	}

	void UIGlyphLine::setGlyphsCount(int s)
	{
		glyphsCount_ = s;
	}

	size_t UIGlyphLine::layoutGlyphSizeForWidth(size_t width)
	{
		size_t w = 0;
		size_t i = 0;
		while (i < glyphsCount_)
		{
			w += glyphs_[i].x_advance;
			if (w > width)
				break;
			++i;
		}
		return i;
	}

	size_t UIGlyphLine::layoutWordSizeForWidth(size_t width)
	{
		size_t lastCount = glyphsCount_;
		size_t w = 0;
		size_t pos = 0;
		while (pos < glyphsCount_)
		{
			bool isSpaceWord = false;
			bool isEndLine = false;
			size_t wordCount = nextWord(buffer_, pos, glyphsCount_, isSpaceWord, isEndLine);

			size_t wordWidth = 0;
			for (size_t i = 0; i < wordCount; ++i)
			{
				wordWidth += glyphs_[pos + i].x_advance;
			}
			if (w + wordWidth > width) {
				if (isSpaceWord)
					return pos + wordCount;
				else
					return pos;
			}
			pos += wordCount;
			w += wordWidth;
		}

		return pos;
	}

	void UIGlyphLine::Render(UIRenderContext* context, Color color)
	{
		context->ShowGlyphs(glyphs_, glyphsCount_, font_, color);
	}

	size_t UIGlyphLine::calcTextWidth() const
	{
		size_t w = 0;
		for (size_t i = 0; i < glyphsCount_; ++i)
		{
			w += glyphs_[i].x_advance;
		}
		return w;
	}

	Size UIGlyphLine::calcTextSize() const
	{
		return Size(calcTextWidth(), calcTextHeight());
	}

	size_t UIGlyphLine::calcTextHeight() const
	{
		return font_->GetHeight();
	}

}