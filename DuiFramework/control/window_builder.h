#pragma once
#include "third_party/tinyxml2.h"

namespace ui
{
	class Window;
	class View;
	class Container;
	class WindowBuilder
	{
	public:
		WindowBuilder(Window* doc, tinyxml2::XMLDocument* xml);
		~WindowBuilder();

		void Run();

		View* BuildView(tinyxml2::XMLElement* e);

		void BuildWindow(tinyxml2::XMLElement* xml_element);

		void BuildTree(Container* container, tinyxml2::XMLElement* xml_element);

		void BuildContainer(View* v, tinyxml2::XMLElement* xml_element);
		void BuildViewAttribute(View* v, tinyxml2::XMLElement* xml_element);

		void BuildBackground(View* v, tinyxml2::XMLElement* xml_element);
		void BuildBorder(View* v, tinyxml2::XMLElement* xml_element);

		void BuildStyleSheet(tinyxml2::XMLElement* xml_element);

		void InitStyleSheet();
	private:
		Window* window_;
		tinyxml2::XMLDocument* xml_;
	};
}