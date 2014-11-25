#include "stdafx.h"
#include "dom_element.h"
#include "dom_include.h"
#include "render/render_include.h"
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

	void UIElement::AttatchRender()
	{
		if (render_obj_)
			return;
		styles_.reset(new RenderStyles(this));
		render_obj_.reset(RenderObject::Create(this));
		if (!render_obj_)
			return;

		UIElement* p = dynamic_cast<UIElement*>(parent().get());
		if (p)
		{
			RenderBox* obj = p->GetRenderBox();
			//obj get list
			if (obj && obj->GetRenderChild()) {
				obj->GetRenderChild()->Append(render_obj_.get());
			}
		}

		if (!HasChildren())
			return;

		for (UINodePtr node = firstChild(); node; node = node->nextSibling())
		{
			//if (node->)
		}
	}

	void UIElement::DetachRender()
	{

	}

	RenderBox* UIElement::GetRenderBox()
	{
		return dynamic_cast<RenderBox*>(render_obj_.get());
	}

	RenderStyles* UIElement::GetRenderStyles()
	{
		return styles_.get();
	}

	Rect UIElement::GetRenderBounds()
	{
		return GetRenderBox()->bounds();
	}

	Rect UIElement::GetContentBounds()
	{
		//ÐèÒª¼õÈ¥border
		return GetRenderBox()->bounds();
	}

	

}