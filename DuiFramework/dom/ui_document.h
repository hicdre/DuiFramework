#pragma once
#include "base/basictypes.h"
#include "dom/ui_forward.h"
#include "render/render_forward.h"
#include "style/style_sheet.h"
#include <string>

namespace ui
{
	class Widget;
	class UIDocument : public RefCounted<UIDocument>
	{
		friend class UIElement;
		friend class DOMDocumentBuilder;
	public:
		UIDocument(const URL& url);
		~UIDocument();

		UIElementPtr RootElement();
		void SetRootElement(UIElementPtr elem);
		const URL& url() const;

		void AddStyleSheet(StyleSheet* s);
		void SelectStyles(UIElement* elem, StyleResultList* styles);

		void UpdateStyles();

		Widget* CreateWidget();

		Widget* GetWidget();
	private:
		UIElementPtr root_element_;
		URL url_;

		Widget* widget_;

		StyleSheetList style_sheets_;
		DISALLOW_COPY_AND_ASSIGN(UIDocument);
	};
}
