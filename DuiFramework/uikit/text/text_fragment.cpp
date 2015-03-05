#include "stdafx.h"
#include "text_fragment.h"
#include "uikit/render/ui_render_font.h"

namespace ui
{

	UITextFragment::UITextFragment(const wchar_t* text, size_t begin, size_t end)
		: text_(text)
		, begin_(begin)
		, end_(end)
		, font_(UIFont::CreateDefault())
	{
		assert(begin < end);
	}

	UITextFragment::~UITextFragment()
	{

	}

	void UITextFragment::setFont(UIFont* font)
	{
		if (font->IsEqual(font))
			return;
		font_.reset(font);
		need_update_glyphs_ = true;
	}

	void UITextFragment::setTextColor(Color color)
	{
		textColor_ = color;
	}

	void UITextFragment::setBackgroundColor(Color color)
	{
		backgroundColor_ = color;
	}

	bool UITextFragment::propertyEquals(UITextFragment* fragment)
	{
		return textColor_ == fragment->textColor_
			&& backgroundColor_ == fragment->backgroundColor_
			&& font_->IsEqual(fragment->font_);
	}

	Size UITextFragment::textSize()
	{
		return Size(textWidth(), textHeight());
	}

	void UITextFragment::updateGlyphs()
	{
		if (!need_update_glyphs_)
			return;

		clearGlyphs();
		scoped_refptr<UIRenderFont> renderFont(font_->renderFont());
		renderFont->GetGlyphs(str(), strLength(),
			&glyphs_, glyphsCount_);

		need_update_glyphs_ = true;
	}

	void UITextFragment::clearGlyphs()
	{
		if (glyphs_)
		{
			delete [] glyphs_;
			glyphs_ = NULL;
			glyphsCount_ = 0;
		}
		
	}

	int UITextFragment::textHeight()
	{
		return font_->renderFont()->GetHeight();
	}

	int UITextFragment::textWidth()
	{
		updateGlyphs();
		int w = 0;
		for (size_t i = 0; i < glyphsCount_; ++i)
		{
			w += glyphs_[i].x_advance;
		}
		return w;
	}

	void UITextFragment::Render(UIRenderContext* context)
	{
		updateGlyphs();
		if (backgroundColor_ != Color_Transparent) {
			context->FillRect(Rect(textWidth(), textHeight()), backgroundColor_);
		}
		context->ShowGlyphs(glyphs_, glyphsCount_, font_->renderFont(), textColor_);
	}

}