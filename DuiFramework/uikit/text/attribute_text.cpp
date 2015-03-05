#include "stdafx.h"
#include "attribute_text.h"

namespace ui
{


	UIAttriubeText::UIAttriubeText(const std::wstring& text)
		: text_(text)
	{
		if (!text_.empty()) {
			element_ = new UITextAttributeElement(Range(0, text.size()));//默认属性
		}
	}

	UIAttriubeText::UIAttriubeText()
		: element_(NULL)
	{

	}

	UIAttriubeText::~UIAttriubeText()
	{
		if (element_) {
			delete element_;
			element_ = NULL;
		}
	}

	void UIAttriubeText::setAttributes(const Range& range, UITextAttributes* attributes)
	{
		if (!element_)
			return;

		element_->updateAttributes(range.start(), range.length(), attributes);
	}

}