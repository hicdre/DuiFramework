#include "stdafx.h"
#include "ui_parser.h"
#include "resource_loader.h"

#include "ui/ui_include.h"
#include "utils/utils.h"
#include "third_party/tinyxml2.h"


#include <functional>

namespace ui
{

	using namespace tinyxml2;

	UIParser::UIParser()
	{

	}

	void UIParser::SetDomDocument(UIDocumentPtr document)
	{
		document_ = document;
	}

	bool UIParser::ParseXmlDocument(const std::string& buffer)
	{
		tinyxml2::XMLDocument xml;
		if (XML_SUCCESS != xml.Parse(buffer.c_str(), buffer.size()))
			return false;

		tinyxml2::XMLElement* elem = xml.RootElement();
		UIElementPtr root = ParseUIElement(elem, elem->Name());
		document_->SetRootElement(root);
		document_->UpdateStyles();
		return true;
	}


	bool UIParser::ParseElementClasses(UIElementPtr elem, const char* val)
	{
		while (true)
		{
			while (*val && isspace(*val))
				++val;
			if (!*val)
				break;

			const char* begin = val;

			while (*val && (isalpha(*val) || isdigit(*val) || *val == '_' || *val == '-'))
				++val;

			const char* end = val;
			if (begin < end)
				elem->addClass(std::string(begin, end - begin));
		}
		return true;
	}


	ui::UIElementPtr UIParser::ParseUIElement(void* p, const char* tag)
	{
		//if (!_strcmpi(tag, "Text"))
		//	return ParseUITextElement(p);

		UIElementPtr elem;
		if (!_strcmpi(tag, "Window"))
			elem = new UIWindow(document_);
		else if(!_strcmpi(tag, "HBox"))
			elem = new UIHBox(document_);
		else if(!_strcmpi(tag, "VBox"))
			elem = new UIVBox(document_);
		else
			elem = new UIElement(document_);

		InitUIElementAttributes(elem.get(), p);
		InitUIElementChild(elem.get(), p);
		return elem;
	}

	void UIParser::ParseStyleSheet(void* p)
	{
		tinyxml2::XMLElement* xml_element = static_cast<tinyxml2::XMLElement*>(p);
		const char* file = xml_element->Attribute("src");
		if (!file)
			return;

		//URL url = document_->url().Resolve(file);
		URL url(document_->url().Resolve(file));
		scoped_refptr<StyleSheet>  sheet = 
			ResourceLoader::Get()->GetStyleSheet(url);
		document_->AddStyleSheet(sheet.get());
	}

	void UIParser::InitUIElementAttributes(UIElement* current, void* elem)
	{
		tinyxml2::XMLElement* xml_element = static_cast<tinyxml2::XMLElement*>(elem);
		{
			const char* val = xml_element->Attribute("id");
			if (val)
			{
				current->setId(val);
			}
		}
		{
			const char* val = xml_element->Attribute("class");
			if (val)
			{
				ParseElementClasses(current, val);
			}
		}
	}

	ui::UIElementPtr UIParser::ParseUITextElement(void* elem)
	{
		tinyxml2::XMLElement* xml_element = static_cast<tinyxml2::XMLElement*>(elem);
		scoped_refptr<UIText> text(new UIText(document_));

		InitUIElementAttributes(text.get(), elem);

		{
			const char* val = xml_element->Attribute("text-raw");

			if (val){
				text->SetText(MultiByteToWide(val));
			}
		}


		return UIElementPtr(text.get());
	}

	void UIParser::ParseUIElementChild(UIElement* current, void* child)
	{
		tinyxml2::XMLElement* e = static_cast<tinyxml2::XMLElement*>(child);
		const char* xml_tag = e->Name();

		if (!_strcmpi(xml_tag, "StyleSheet")) {
			ParseStyleSheet(e);
		}
		else {
			UIElementPtr child = ParseUIElement(e, xml_tag);
			if (child)
				current->Append(UIElementPtr(child.get()));
		}
	}


	void UIParser::InitUIElementChild(UIElement* current, void* elem)
	{
		tinyxml2::XMLElement* e = static_cast<tinyxml2::XMLElement*>(elem);
		for (tinyxml2::XMLElement* el = e->FirstChildElement();
			el; el = el->NextSiblingElement())
		{
			ParseUIElementChild(current, el);
		}
	}
}