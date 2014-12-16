#include "stdafx.h"
#include "ui_document.h"
#include "ui_include.h"
#include "render/render_include.h"
#include "utils/utils.h"

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
		UIWindow* window = dynamic_cast<UIWindow*>(RootElement().get());
		window->AttatchWidget(widget_);
		window->LayoutIfNeeded();
		return widget_;
	}

	void UIDocument::SelectStyles(UIElement* elem, StyleResultList* styles)
	{
		style_sheets_.SelectStyles(elem, styles);
	}

	void UIDocument::UpdateStyles()
	{
		root_element_->UpdateStyles();
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

