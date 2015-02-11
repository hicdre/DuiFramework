#include "stdafx.h"
#include "ui_attribute_string.h"

namespace ui
{
	
	UIAttributeString::UIAttributeString()
	{

	}

	UIAttributeString::UIAttributeString(const std::wstring& contents)
		: string_(contents)
	{

	}

	UIAttributeString::~UIAttributeString()
	{

	}

	UIColorRange* UIAttributeString::backgroundColorRange()
	{
		if (backgroundColorRange_.get())
			return backgroundColorRange_.get();

		if (string_.empty())
			return NULL;

		backgroundColorRange_.reset(new UIColorRange(string_.size(), Color_Transparent));
		return backgroundColorRange_.get();
	}

	UIColorRange* UIAttributeString::textColorRange()
	{
		if (textColorRange_.get())
			return textColorRange_.get();

		if (string_.empty())
			return NULL;

		textColorRange_.reset(new UIColorRange(string_.size(), Color_Black));
		return textColorRange_.get();
	}


	UIFontRange* UIAttributeString::fontRange()
	{
		if (fontRange_.get())
			return fontRange_.get();

		if (string_.empty())
			return NULL;

		fontRange_.reset(new UIFontRange(string_.size(), UIFont::CreateDefault()));
		return fontRange_.get();
	}


	void UIAttributeString::setBackgroundColorInRange(Color color, const Range& range)
	{
		UIColorRange* uiRange = backgroundColorRange();
		if (uiRange == NULL)
			return;

		uiRange->Update(range, color);
	}

	void UIAttributeString::setTextColorInRange(Color color, const Range& range)
	{
		UIColorRange* uiRange = textColorRange();
		if (uiRange == NULL)
			return;

		uiRange->Update(range, color);
	}

	void UIAttributeString::setFontInRange(UIFont* font, const Range& range)
	{
		UIFontRange* uiRange = fontRange();
		if (uiRange == NULL)
			return;

		uiRange->Update(range, font);
	}

}