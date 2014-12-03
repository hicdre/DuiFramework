#pragma once
#include "dom/ui_element.h"

namespace ui
{
	class UIVBox : public UIElement
	{
	public:
		UIVBox(UIDocumentPtr doc);
		~UIVBox();

		virtual void Layout() override;
	};
}