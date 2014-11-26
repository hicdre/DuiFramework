#include "stdafx.h"
#include "dom_document.h"
#include "dom_include.h"
#include "utils/utils.h"
#include "render/render_widget.h"

namespace ui
{

	UIDocument::UIDocument(const URL& url)
		: url_(url)
		, widget_(NULL)
	{
	}

	UIDocument::~UIDocument()
	{
	}

	UIElementPtr UIDocument::RootElement()
	{
		return root_element_;
	}

	const URL& UIDocument::url() const
	{
		return url_;
	}

	void UIDocument::SetRootElement(UIElementPtr elem)
	{
		root_element_ = elem;
	}

	void UIDocument::AddStyleSheet(StyleSheet* s)
	{
		style_sheets_.AddStyleSheet(s);
	}

	Widget* UIDocument::GetWidget()
	{
		return widget_;
	}

	Widget* UIDocument::CreateWidget()
	{
		if (widget_)
			return widget_;
		widget_ = Widget::Create();
		UIElementPtr root = RootElement();
		root->AttatchRender();
		root->GetRenderObject()->Layout();
		return widget_;
	}

	void UIDocument::SelectStyles(UIElement* elem, RenderStyles* styles)
	{
		style_sheets_.SelectStyles(elem, styles);
	}

// 	void UIDocument::Load(const std::string& str)
// 	{
// 		tinyxml2::XMLDocument xml;
// 		if (XML_SUCCESS == xml.Parse(str.c_str()))
// 		{
// 			DOMDocumentBuilder bulider(this, &xml);
// 			bulider.Run();
// 		}
// 	}
// 
// 	void UIDocument::LoadFile(const std::wstring& file)
// 	{
// 		tinyxml2::XMLDocument xml;
// 		if (XML_SUCCESS == xml.LoadFile(WideToMultiByte(file).c_str()))
// 		{
// 			DOMDocumentBuilder bulider(this, &xml);
// 			bulider.Run();
// 		}
// 	}

}

