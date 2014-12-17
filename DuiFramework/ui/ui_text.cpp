#include "stdafx.h"
#include "ui_text.h"

#include "utils/utils.h"

namespace ui
{


	UIText::UIText(UIDocumentPtr doc)
		: UIElement(doc)
	{
		tag_ = "Text";
	}

	UIText::~UIText()
	{

	}

	void UIText::SetText(const std::wstring& text)
	{
		text_ = text;
	}

	void UIText::PaintContents(RenderContext* painter)
	{
		//TextLayout layout(text_)
		//painter->
	}

}