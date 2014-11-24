#pragma once
#include "base/basictypes.h"
#include "dom/dom_node.h"
#include "dom/dom_element.h"
#include "style/style_sheet.h"
#include <string>

namespace ui
{
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
	private:
		UIElementPtr root_element_;
		URL url_;

		StyleSheetList style_sheets_;
		DISALLOW_COPY_AND_ASSIGN(UIDocument);
	};
}
