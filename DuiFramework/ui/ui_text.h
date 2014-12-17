#pragma once
#include "ui/ui_element.h"

namespace ui
{
	//no child
	class UIText : public UIElement
	{
		friend class UIElement;
	public:
		UIText(UIDocumentPtr doc);
		~UIText();

		void SetText(const std::wstring& text);

		virtual void PaintContents(RenderContext* painter) override;
	private:
		std::wstring text_;
	};
}