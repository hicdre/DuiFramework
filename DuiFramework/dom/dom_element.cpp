#include "stdafx.h"
#include "dom_element.h"

namespace ui
{


	DOMElement::DOMElement(DOMDocument* doc)
		: DOMNode(doc)
	{

	}

	DOMElement::~DOMElement()
	{

	}

	const char* DOMElement::getTag() const
	{
		return NULL;
	}

	void DOMElement::setId(const std::string& id)
	{
		id_ = id;
	}

	const std::string& DOMElement::getId() const
	{
		return id_;
	}

	void DOMElement::addClass(const std::string& v)
	{
		classes_.insert(v);
	}

	void DOMElement::removeClass(const std::string& v)
	{
		classes_.erase(v);
	}

	bool DOMElement::haveClass(const std::string& v) const
	{
		return classes_.count(v) > 0;
	}

	void DOMElement::clearClass()
	{
		classes_.clear();
	}

}