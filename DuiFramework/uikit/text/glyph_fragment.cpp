#include "stdafx.h"
#include "glyph_fragment.h"
#include "uikit/render/ui_render_font.h"

namespace ui
{


	UIGlyphFragment::UIGlyphFragment(UITextFragment* text,
		size_t begin, size_t end) 
		: textFragment_(text)
		, begin_(begin)
		, end_(end)
	{
		assert(begin < end);
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
		return end_ - begin_;
	}

	int UIGlyphFragment::width() const
	{
		if (!textFragment_)
			return 0;
		return textFragment_->textWidthWithRange(begin_, end_);
	}

	void UIGlyphFragment::Render(UIRenderContext* context)
	{
		if (!textFragment_)
			return;
		textFragment_->RenderWithRange(context, begin_, end_);
	}

	int UIGlyphFragment::height() const
	{
		if (!textFragment_)
			return 0;
		return textFragment_->textHeight();
	}

}