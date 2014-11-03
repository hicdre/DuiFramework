#include "stdafx.h"
#include "dom_node.h"

namespace ui
{
	DOMNode* DOMNode::lastChild() const
	{
		return last_child_;
	}

	DOMNode* DOMNode::firstChild() const
	{
		return first_child_;
	}

	DOMNode* DOMNode::nextSibling() const
	{
		return next_sibling_;
	}

	DOMNode* DOMNode::previousSibling() const
	{
		return prev_sibling_;
	}

	bool DOMNode::HasChildren() const
	{
		return !!first_child_;
	}

	DOMNode* DOMNode::parent()
	{
		return parent_;
	}

	DOMDocument* DOMNode::GetDocument()
	{
		return document_;
	}

	DOMNode::DOMNode(DOMDocument* doc)
		: document_(doc)
	{

	}
	DOMNode::~DOMNode()
	{
		DeleteChildren();
	}

	DOMNode* DOMNode::AppendTo(DOMNode* parent)
	{
		return parent->Append(this);
	}

	DOMNode* DOMNode::Detach()
	{
		return parent_->Remove(this);
	}

	DOMNode* DOMNode::Append(DOMNode* child)
	{
		return InsertAfter(last_child_, child);
	}

	DOMNode* DOMNode::Remove(DOMNode* child)
	{
		//只允许移除子节点
		if (!child || child->parent_ != this)
		{
			assert(0); //<< "can only remove child node";
			return NULL;
		}

		//更新父子关系
		child->parent_ = NULL;
		if (first_child_ == child)
		{
			first_child_ = child->next_sibling_;
		}
		if (last_child_ == child)
		{
			last_child_ = child->prev_sibling_;
		}

		//更新兄弟关系
		if (child->prev_sibling_)
		{
			child->prev_sibling_->next_sibling_ = child->next_sibling_;
		}

		if (child->next_sibling_)
		{
			child->next_sibling_->prev_sibling_ = child->prev_sibling_;
		}

		child->next_sibling_ = NULL;
		child->prev_sibling_ = NULL;

		//child_count_--;

		return child;
	}

	DOMNode* DOMNode::InsertAfter(DOMNode* ref, DOMNode* child)
	{
		if (ref == child || child == NULL || (ref && ref->parent_ != this))
		{
			assert(0);
			return NULL;
		}

		//先从原节点移除
		if (child->parent_)
		{
			child->parent_->Remove(child);
		}
		child->parent_ = this;

		child->prev_sibling_ = ref;
		if (ref)
		{//插到中间或最后
			child->next_sibling_ = ref->next_sibling_;
			if (ref->next_sibling_)
			{
				ref->next_sibling_->prev_sibling_ = child;
			}
			ref->next_sibling_ = child;

			assert(last_child_);
			//插到最后了
			if (last_child_->next_sibling_)
			{
				last_child_ = last_child_->next_sibling_;
			}
		}
		else
		{//插到最前
			child->next_sibling_ = first_child_;
			if (first_child_)
			{
				first_child_->prev_sibling_ = child;
			}
			first_child_ = child;

			//原来没有子元素
			if (last_child_ == NULL)
			{
				last_child_ = child;
			}
		}
		//child_count_++;

		return child;
	}

	DOMNode* DOMNode::InsertBefore(DOMNode* ref, DOMNode* child)
	{
		if (ref == child || child == NULL || (ref && ref->parent_ != this))
		{
			assert(0);// << "insert before invalid";
			return NULL;
		}

		if (ref)
		{
			return InsertAfter(ref->prev_sibling_, child);
		}
		else
		{
			return InsertAfter(last_child_, child);
		}
	}

	void DOMNode::DeleteChildren()
	{
		while (first_child_) {
			DOMNode* node = first_child_;
			DeleteChild(node);
		}
	}

	void DOMNode::DeleteChild(DOMNode* n)
	{
		DOMNode* node = Remove(n);
		if (node) {
			delete node;
		}
	}



}