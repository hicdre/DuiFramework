#include "stdafx.h"
#include "dom_node.h"
#include "dom_element.h"
#include "dom_document.h"

namespace ui
{
	UINode::UINode(UIDocumentPtr doc)
		: document_(doc)
	{

	}

	UINode::~UINode()
	{
		RemoveChildren();
	}

	UIDocumentPtr UINode::GetDocument()
	{
		return document_;
	}

	UINodePtr UINode::lastChild() const
	{
		return last_child_;
	}

	UINodePtr UINode::firstChild() const
	{
		return first_child_;
	}

	UINodePtr UINode::nextSibling() const
	{
		return next_sibling_;
	}

	UINodePtr UINode::previousSibling() const
	{
		return prev_sibling_;
	}

	bool UINode::HasChildren() const
	{
		return !!first_child_;
	}

	UINodePtr UINode::parent()
	{
		return parent_;
	}

	void UINode::AppendTo(UINodePtr parent)
	{
		parent_->Append(this);
	}

	void UINode::Detach()
	{
		if (parent_)
			parent_->Remove(this);
	}

	void UINode::Append(UINodePtr child)
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

		child->parent_ = UINodePtr(this);
	}


	void UINode::Prepend(UINodePtr child)
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

		child->parent_ = UINodePtr(this);
	}

	void UINode::Unlink(UINodePtr child)
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

	void UINode::Remove(UINodePtr child)
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

	void UINode::InsertAfterChild(UINodePtr ref, UINodePtr child)
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

		child->parent_ = UINodePtr(this);
	}

	void UINode::InsertBeforeChild(UINodePtr ref, UINodePtr child)
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

	void UINode::RemoveChildren()
	{
		while (first_child_) {
			UINodePtr node = first_child_;

			Unlink(node);
		}
	}

}