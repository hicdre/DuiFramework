#pragma once
#include "base/basictypes.h"
#include "ui/ui_document.h"

namespace ui
{
	class UIParser
	{
	public:
		UIParser();
		
		void SetDomDocument(UIDocumentPtr document);

		bool ParseXmlDocument(const std::string& buffer);
	private:
		void ParseXmlElement(void* elem);
		UIElementPtr ParseUIElement(void* elem, const char* tag);
		UIElementPtr ParseUITextElement(void* elem);

		void InitUIElementAttributes(UIElement* current, void* elem);
		void InitUIElementChild(UIElement* current, void* elem);
		void ParseUIElementChild(UIElement* current, void* child);
		void ParseStyleSheet(void* elem);

		bool ParseElementClasses(UIElementPtr elem, const char* val);
		UIDocumentPtr document_;
		DISALLOW_COPY_AND_ASSIGN(UIParser);
	};
}