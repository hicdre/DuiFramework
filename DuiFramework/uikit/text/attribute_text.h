#pragma once
#include "uikit/text/text_attribute_element.h"

namespace ui
{
	class UIAttriubeText
	{
	public:
		UIAttriubeText();
		UIAttriubeText(const std::wstring& text);
		~UIAttriubeText();

		void setAttributes(const Range& range, UITextAttributes* attributes);


	private:
		std::wstring text_;
		UITextAttributeElement* element_;
	};
}