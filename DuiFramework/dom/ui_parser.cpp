#include "stdafx.h"
#include "ui_parser.h"
#include "ui_include.h"
#include "third_party/tinyxml2.h"
#include "utils/resource_loader.h"
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

		UIElementPtr elem = ParseXmlElement(xml.RootElement());
		document_->SetRootElement(elem);
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

	UIElementPtr UIParser::ParseXmlElement(void* elem)
	{
		tinyxml2::XMLElement* e = static_cast<tinyxml2::XMLElement*>(elem);
		const char* xml_tag = e->Name();

		if (!_strcmpi(xml_tag, "StyleSheet")) {
			ParseStyleSheet(elem);
			return NULL;
		}
		else {
			return ParseUIElement(elem, xml_tag);
		}
	}


	ui::UIElement* UIParser::CreateUIElement(const char* tag)
	{
		if (!_strcmpi(tag, "Window"))
			return new UIWindow(document_);
		if (!_strcmpi(tag, "HBox"))
			return new UIHBox(document_);
		if (!_strcmpi(tag, "VBox"))
			return new UIVBox(document_);
		return new UIElement(document_);
	}


	ui::UIElementPtr UIParser::ParseUIElement(void* p, const char* tag)
	{
		tinyxml2::XMLElement* e = static_cast<tinyxml2::XMLElement*>(p);

		UIElementPtr elem(CreateUIElement(tag));
		elem->setTag(tag);
		{
			const char* val = e->Attribute("id");
			if (val)
			{
				elem->setId(val);
			}
		}
		{
			const char* val = e->Attribute("class");
			if (val)
			{
				ParseElementClasses(elem, val);
			}
		}

		for (tinyxml2::XMLElement* el = e->FirstChildElement();
			el; el = el->NextSiblingElement())
		{
			UIElementPtr child = ParseXmlElement(el);
			if (child)
				elem->Append(UIElementPtr(child.get()));
		}
		return elem;
	}

	void UIParser::ParseStyleSheet(void* p)
	{
		tinyxml2::XMLElement* xml_element = static_cast<tinyxml2::XMLElement*>(p);
		const char* file = xml_element->Attribute("src");
		if (!file)
			return;

		URL url = document_->url().Resolve(file);
		scoped_refptr<StyleSheet>  sheet = 
			ResourceLoader::Get()->GetStyleSheet(url);
		document_->AddStyleSheet(sheet.get());
	}

}