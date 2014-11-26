#include "stdafx.h"
#include "dom_element.h"
#include "dom_include.h"

#include "render/render_include.h"

namespace ui
{
	UIElement::UIElement(UIDocumentPtr doc)
		: document_(doc)
	{

	}

	UIElement::~UIElement()
	{
		RemoveChildren();
	}

	UIDocumentPtr UIElement::GetDocument()
	{
		return document_;
	}

	UIElementPtr UIElement::lastChild() const
	{
		return last_child_;
	}

	UIElementPtr UIElement::firstChild() const
	{
		return first_child_;
	}

	UIElementPtr UIElement::nextSibling() const
	{
		return next_sibling_;
	}

	UIElementPtr UIElement::previousSibling() const
	{
		return prev_sibling_;
	}

	bool UIElement::HasChildren() const
	{
		return !!first_child_;
	}


	bool UIElement::HasParent() const
	{
		return !!parent_;
	}


	UIElementPtr UIElement::parent()
	{
		return parent_;
	}

	void UIElement::AppendTo(UIElementPtr parent)
	{
		parent_->Append(this);
	}

	void UIElement::Detach()
	{
		if (parent_)
			parent_->Remove(this);
	}

	void UIElement::Append(UIElementPtr child)
	{
		if (child->document_ != document_) {
			assert(0);
			return;
		}

		if (child->parent_) {
			child->parent_->Unlink(child);
		}

		if (last_child_) {
			last_child_->next_sibling_ = child;
			child->prev_sibling_ = last_child_;
			last_child_ = child;
			child->next_sibling_ = NULL;
		}
		else {
			first_child_ = child;
			last_child_ = child;

			child->prev_sibling_ = NULL;
			child->next_sibling_ = NULL;
		}

		child->parent_ = UIElementPtr(this);
	}


	void UIElement::Prepend(UIElementPtr child)
	{
		if (child->document_ != document_) {
			assert(0);
			return;
		}

		if (child->parent_) {
			child->parent_->Unlink(child);
		}

		if (first_child_) {
			first_child_->prev_sibling_ = child;
			child->next_sibling_ = first_child_;
			first_child_ = child;
			child->prev_sibling_ = NULL;
		}
		else {
			first_child_ = child;
			last_child_ = child;

			child->prev_sibling_ = NULL;
			child->next_sibling_ = NULL;
		}

		child->parent_ = UIElementPtr(this);
	}

	void UIElement::Unlink(UIElementPtr child)
	{
		if (first_child_.get() == child.get()) {
			first_child_ = first_child_->next_sibling_;
		}

		if (last_child_.get() == child.get()) {
			last_child_ = last_child_->prev_sibling_;
		}

		if (child->prev_sibling_) {
			child->prev_sibling_->next_sibling_ = child->next_sibling_;
		}
		if (child->next_sibling_) {
			child->next_sibling_->prev_sibling_ = child->prev_sibling_;
		}
		child->parent_.reset();
	}

	void UIElement::Remove(UIElementPtr child)
	{
		//只允许移除子节点
		if (!child || child->parent_.get() != this)
		{
			assert(0); //<< "can only remove child node";
			return;
		}

		Unlink(child);

		child->prev_sibling_ = NULL;
		child->next_sibling_ = NULL;
	}

	void UIElement::InsertAfterChild(UIElementPtr ref, UIElementPtr child)
	{
		if (ref == child || child == NULL || (ref && ref->parent_.get() != this))
		{
			assert(0);
			return;
		}

		if (!ref->next_sibling_) {
			Append(child);
			return;
		}

		if (child->parent_)
			child->parent_->Unlink(child);

		child->prev_sibling_ = ref;
		child->next_sibling_ = ref->next_sibling_;
		ref->next_sibling_->prev_sibling_ = child;
		ref->next_sibling_ = ref;

		child->parent_ = UIElementPtr(this);
	}

	void UIElement::InsertBeforeChild(UIElementPtr ref, UIElementPtr child)
	{
		if (ref == child || child.get() == NULL || (ref && ref->parent_.get() != this))
		{
			assert(0);// << "insert before invalid";
			return;
		}

		if (ref->prev_sibling_)
		{
			return InsertAfterChild(ref->prev_sibling_, child);
		}
		else
		{
			return Prepend(child);
		}
	}

	void UIElement::RemoveChildren()
	{
		while (first_child_) {
			UIElementPtr node = first_child_;

			Unlink(node);
		}
	}

	RenderObject* UIElement::GetRenderObject()
	{
		return render_obj_.get();
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
		render_obj_.reset(RenderObject::Create(this, styles_.get()));
		if (!render_obj_)
			return;

		UIElement* p = dynamic_cast<UIElement*>(parent().get());
		if (p)
		{
			RenderBox* obj = p->GetRenderBox();
			//obj get list
			if (obj) {
				obj->addChild(render_obj_.get());
			}
		}

		if (!HasChildren())
			return;

		for (UIElementPtr node = firstChild(); node; node = node->nextSibling())
		{
			node->AttatchRender();
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
		//需要减去border
		return GetRenderBox()->bounds();
	}

}