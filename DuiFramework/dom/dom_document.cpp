#include "stdafx.h"
#include "dom_document.h"
#include "dom_element.h"
#include "utils/utils.h"
#include "third_party/tinyxml2.h"

namespace ui
{
	using namespace tinyxml2;

	class DOMDocumentBuilder
	{
	public:
		DOMDocumentBuilder(DOMDocument* doc, tinyxml2::XMLDocument* xml)
			: doc_(doc), xml_(xml) {}
		~DOMDocumentBuilder() {}

		void Run()
		{
			tinyxml2::XMLElement* xml_element = xml_->RootElement();
			DOMElement* dom_element = CreateDOMElement(xml_element);
			doc_->Append(dom_element);

			BuildTree(dom_element, xml_element);
			
		}

		DOMElement* CreateDOMElement(tinyxml2::XMLElement* e)
		{
			DOMElement* elem = new DOMElement(doc_);

			//elem->setTag(e->Name());
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
				}
			}
			return elem;
		}

		void BuildTree(DOMElement* dom_element, tinyxml2::XMLElement* xml_element)
		{
			for (tinyxml2::XMLElement* e = xml_element->FirstChildElement();
				e; e = e->NextSiblingElement())
			{
				DOMElement* n = CreateDOMElement(e);
				dom_element->Append(n);
				BuildTree(n, e);
			}
		}
	private:
		DOMDocument* doc_;
		tinyxml2::XMLDocument* xml_;
	};

	DOMDocument::DOMDocument()
		: DOMNode(NULL)
	{
		document_ = this;
	}

	DOMDocument::~DOMDocument()
	{

	}

	DOMElement* DOMDocument::RootElement()
	{
		return NULL;
	}

	void DOMDocument::Load(const std::string& str)
	{
		tinyxml2::XMLDocument xml;
		if (XML_SUCCESS == xml.Parse(str.c_str()))
		{
			DOMDocumentBuilder bulider(this, &xml);
			bulider.Run();
		}
	}

	void DOMDocument::LoadFile(const std::wstring& file)
	{
		tinyxml2::XMLDocument xml;
		if (XML_SUCCESS == xml.LoadFile(WideToMultiByte(file).c_str()))
		{
			DOMDocumentBuilder bulider(this, &xml);
			bulider.Run();
		}
	}

}

