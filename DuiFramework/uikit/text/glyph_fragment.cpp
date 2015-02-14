#include "stdafx.h"
#include "glyph_fragment.h"
#include "uikit/render/ui_render_font.h"

namespace ui
{


	UIGlyphFragment::UIGlyphFragment(UIGlyph* glyphs, const wchar_t* buffer, size_t size, UIFont* font) : glyphs_(glyphs)
		, glyphsCount_(size)
		, font_(font)
		, string_(buffer)
	{

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

	void UIGlyphFragment::setGlyphsCount(int s)
	{
		glyphsCount_ = s;
	}

	size_t UIGlyphFragment::calcTextWidth() const
	{
		size_t w = 0;
		for (size_t i = 0; i < glyphsCount_; ++i)
		{
			w += glyphs_[i].x_advance;
		}
		return w;
	}

	Size UIGlyphFragment::calcTextSize() const
	{
		return Size(calcTextWidth(), font_->renderFont()->GetHeight());
	}

	void UIGlyphFragment::Render(UIRenderContext* context, Color color)
	{
		context->ShowGlyphs(glyphs_, glyphsCount_, font_->renderFont(), color);
	}

}