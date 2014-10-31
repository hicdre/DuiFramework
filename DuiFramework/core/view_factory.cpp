#include "stdafx.h"
#include "view_factory.h"
#include "third_party/tinyxml2.h"
#include "utils/utils.h"

#include "control/button.h"
#include "control/text_view.h"

namespace ui
{


	ViewFactory::ViewFactory()
	{

	}

	ViewFactory::~ViewFactory()
	{

	}

	View* ViewFactory::CreateViewFromXml(const std::wstring& path)
	{
		tinyxml2::XMLDocument doc;
		doc.LoadFile(WideToMultiByte(path).c_str());
		std::list<std::pair<tinyxml2::XMLElement*,View*>> build_stack;
		build_stack.push_back(std::make_pair(doc.RootElement(), (View*)NULL));

		View* ret = NULL;
		while (!build_stack.empty())
		{
			std::pair<tinyxml2::XMLElement*, View*> pair = build_stack.back();
			tinyxml2::XMLElement* elem = pair.first;
			build_stack.pop_back();

			std::string tag_name = elem->Name();
			View* v = NULL;
			if (tag_name == "button")
			{
				v = new TextButton;
			}
			else if (tag_name == "text")
			{
				v = new TextView;
			}
			else if (constructor_map_.count(tag_name))
			{
				v = constructor_map_.at(tag_name)();
			}

			if (!v)
				continue;

			for (const tinyxml2::XMLAttribute* attr = elem->FirstAttribute(); attr;
				attr = attr->Next())
			{
				v->SetPropertyString(attr->Name(), attr->Value());
			}

			if (pair.second)
			{
				pair.second->InsertAfter(NULL, v);
			}
			else
			{
				ret = v;
			}

			for (tinyxml2::XMLElement* c = elem->FirstChildElement(); c;
				c = c->NextSiblingElement())
			{
				build_stack.push_back(std::make_pair(c, v));
			}
		}
		return ret;
	}

	void ViewFactory::RegisterViewClass(const std::string& name, ViewConstructor c)
	{
		constructor_map_[name] = c;
	}

}