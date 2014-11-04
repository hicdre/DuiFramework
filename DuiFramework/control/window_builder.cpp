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
		View* elem = new View;
		BuildContainer(elem, e);
		BuildLayoutBox(elem, e);
		BuildBorder(elem, e);
		BuildBackground(elem, e);
		return elem;
	}

	void WindowBuilder::Run()
	{
		tinyxml2::XMLElement* xml_element = xml_->RootElement();
		BuildWindow(xml_element);
		BuildTree(window_->GetContainer(), xml_element);
	}


	void WindowBuilder::BuildWindow(tinyxml2::XMLElement* xml_element)
	{
		//layout
		BuildContainer(window_, xml_element);
		BuildLayoutBox(window_, xml_element);
		BuildBorder(window_, xml_element);
		BuildBackground(window_, xml_element);
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


	void WindowBuilder::BuildLayoutBox(View* v, tinyxml2::XMLElement* xml_element)
	{
		LayoutBox* box = v->layoutBox();
		//layoutbox
		{
			const char* val = xml_element->Attribute("width");
			if (val)
			{
				//box->setWidth(Length::FromString(val));
			}
		}
		{
			const char* val = xml_element->Attribute("height");
			if (val)
			{
				//box->setHeight(Length::FromString(val));
			}
		}
		{
			std::vector<std::string> strs;
			const char* marginLeft = 0, *marginTop = 0, *marginRight = 0, *marginBottom = 0;

			if (v->IsAbsouletLayout())
			{
				const char* val = xml_element->Attribute("pos");
				if (val)
				{
					spilt_string(val, ',', strs, 2);
					if (strs.size() == 2)
					{
						marginLeft = strs[0].c_str();
						marginTop = strs[1].c_str();
					}
				}
			}
			else
			{
				const char* val = xml_element->Attribute("margin");
				if (val)
				{
					spilt_string(val, ',', strs, 4);
					if (strs.size() == 4)
					{
						marginLeft = strs[0].c_str();
						marginTop = strs[1].c_str();
						marginRight = strs[2].c_str();
						marginBottom = strs[3].c_str();
					}
				}
			}
			
			{
				const char* val = xml_element->Attribute("marginLeft", marginLeft);
				if (val)
				{
					//box->setMarginLeft(Length::FromString(val));
				}
			}
			{
				const char* val = xml_element->Attribute("marginTop", marginTop);
				if (val)
				{
					//box->setMarginTop(Length::FromString(val));
				}
			}
			{
				const char* val = xml_element->Attribute("marginRight", marginRight);
				if (val)
				{
					//box->setMarginRight(Length::FromString(val));
				}
			}
			{
				const char* val = xml_element->Attribute("marginBottom", marginBottom);
				if (val)
				{
					//box->setMarginBottom(Length::FromString(val));
				}
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



}