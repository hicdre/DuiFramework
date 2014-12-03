#pragma once
#include "dom/ui_element.h"

namespace ui
{
	class UIHBox : public UIElement
	{
	public:
		UIHBox(UIDocumentPtr doc);
		~UIHBox();

		virtual void Layout() override;
	};
}