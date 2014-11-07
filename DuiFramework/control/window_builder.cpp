#include "stdafx.h"
#include "window_builder.h"
#include "view/view.h"
#include "utils/utils.h"
#include "window.h"

namespace ui
{
	namespace
	{
		bool StringToColor(const std::string& str, Color& c)
		{
			if (str.size() != 9)
				return false;
			const char* begin = str.c_str();
			if (*begin != '#')
				return false;
			begin++;
			c = strtoul(begin, 0, 16);
			return true;
		}

		//res(xxx)
		//path(xxx)
		//linear-gradient(x%, x%, c, c)
		bool StringToImage(const std::string& str, Image*& image)
		{
			//StringToken
			return false;
		}
	}

	WindowBuilder::WindowBuilder(Window* doc, tinyxml2::XMLDocument* xml) 
		: window_(doc), xml_(xml)
	{

	}

	WindowBuilder::~WindowBuilder()
	{

	}

	void WindowBuilder::BuildTree(Container* container, tinyxml2::XMLElement* xml_element)
	{
		if (!container)
			return;

		for (tinyxml2::XMLElement* e = xml_element->FirstChildElement();
			e; e = e->NextSiblingElement())
		{
			View* n = BuildView(e);
			if (!n)
				continue;
			container->AddChild(n);
			BuildTree(n->GetContainer(), e);
		}
	}

	View* WindowBuilder::BuildView(tinyxml2::XMLElement* e)
	{
		const char* tag_name = e->Name();
		View* elem = NULL;
		if (!strcmp(tag_name,"StyleSheet")) {
			BuildStyleSheet(e);
			return NULL;
		}
		elem = new View;
		BuildViewAttribute(elem, e);
		BuildContainer(elem, e);
		BuildBorder(elem, e);
		BuildBackground(elem, e);
		return elem;
	}

	void WindowBuilder::Run()
	{
		tinyxml2::XMLElement* xml_element = xml_->RootElement();
		BuildWindow(xml_element);
		BuildTree(window_->GetContainer(), xml_element);

		InitStyleSheet();
	}


	void WindowBuilder::BuildWindow(tinyxml2::XMLElement* xml_element)
	{
		//layout
		BuildContainer(window_, xml_element);
		//BuildLayoutBox(window_, xml_element);
		//BuildBorder(window_, xml_element);
		//BuildBackground(window_, xml_element);
	}


	void WindowBuilder::BuildContainer(View* v, tinyxml2::XMLElement* xml_element)
	{
		const char* layout = xml_element->Attribute("layout");
		if (layout && !strcmp(layout, "hbox"))
		{
			v->SetContainer(new HorizonalContainer(v));
		}
		else if (layout && !strcmp(layout, "vbox"))
		{
			v->SetContainer(new VerticalContainer(v));
		}
		else
		{
			v->SetContainer(new AbsoulteContainer(v));
		}
	}


	void WindowBuilder::BuildViewAttribute(View* v, tinyxml2::XMLElement* xml_element)
	{
		{
			const char* val = xml_element->Attribute("id");
			if (val)
			{
				v->SetId(val);
			}
		}
		{
			const char* val = xml_element->Attribute("class");
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
						v->AddClass(std::string(begin, end - begin));
				}
				//box->setHeight(Length::FromString(val));
			}
		}
	}

	void WindowBuilder::BuildBackground(View* v, tinyxml2::XMLElement* xml_element)
	{
		auto getBackground = [v]()->Background*{
			if (!v->background())
			{
				v->SetBackground(new Background);
			}
			return v->background();
		};
		{
			Color c;
			const char* val = xml_element->Attribute("background-color");
			if (val && StringToColor(val, c))
			{
				getBackground()->SetColor(c);
			}
		}
		{
			const char* val = xml_element->Attribute("background-image");
			if (val)
			{
				//getBackground()->SetColor(c);
			}
		}
	}

	void WindowBuilder::BuildBorder(View* v, tinyxml2::XMLElement* xml_element)
	{
		auto getBorder = [v]()->Border*{
			if (!v->border())
			{
				v->SetBorder(new Border);
			}
			return v->border();
		};

		{
			Color c = Color_Transparent;
			int v = xml_element->IntAttribute("border-left-width");
			if (v)
			{
				const char* val = xml_element->Attribute("border-left-color");
				if (val)
				{
					StringToColor(val, c);
				}
				getBorder()->SetBorder(Border::LEFT, v, c);
			}
		}

		{
			Color c = Color_Transparent;
			int v = xml_element->IntAttribute("border-top-width");
			if (v)
			{
				const char* val = xml_element->Attribute("border-top-color");
				if (val)
				{
					StringToColor(val, c);
				}
				getBorder()->SetBorder(Border::TOP, v, c);
			}
		}

		{
			Color c = Color_Transparent;
			int v = xml_element->IntAttribute("border-right-width");
			if (v)
			{
				const char* val = xml_element->Attribute("border-right-color");
				if (val)
				{
					StringToColor(val, c);
				}
				getBorder()->SetBorder(Border::RIGHT, v, c);
			}
		}

		{
			Color c = Color_Transparent;
			int v = xml_element->IntAttribute("border-bottom-width");
			if (v)
			{
				const char* val = xml_element->Attribute("border-bottom-color");
				if (val)
				{
					StringToColor(val, c);
				}
				getBorder()->SetBorder(Border::BOTTOM, v, c);
			}
		}
	}

	void WindowBuilder::BuildStyleSheet(tinyxml2::XMLElement* xml_element)
	{
		const char* file = xml_element->Attribute("src");
		if (file) 
		{
			scoped_refptr<StyleSheet>  sheet = StyleSheet::LoadFromFile(MultiByteToWide(file));
			if (sheet.get())
				window_->AddStyleSheet(sheet.get());
		}
	}

	void WindowBuilder::InitStyleSheet()
	{
		
		StyleSheetList* sheet = const_cast<StyleSheetList*>(window_->GetStyleSheetList());
		window_->UpdateStyleRules(sheet);
	}



}