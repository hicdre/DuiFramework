#include "stdafx.h"
#include "dom_element.h"
#include "dom_document.h"

namespace ui
{


	UIElement::UIElement(UIDocumentPtr doc)
		: UINode(doc)
	{

	}

	UIElement::~UIElement()
	{

	}

	void UIElement::setTag(const std::string& tag)
	{
		tag_ = tag;
	}

	const std::string& UIElement::getTag() const
	{
		return tag_;
	}

	void UIElement::setId(const std::string& id)
	{
		id_ = id;
	}

	const std::string& UIElement::getId() const
	{
		return id_;
	}

	void UIElement::addClass(const std::string& v)
	{
		classes_.insert(v);
	}

	void UIElement::removeClass(const std::string& v)
	{
		classes_.erase(v);
	}

	bool UIElement::haveClass(const std::string& v) const
	{
		return classes_.count(v) > 0;
	}

	void UIElement::clearClass()
	{
		classes_.clear();
	}

	

}