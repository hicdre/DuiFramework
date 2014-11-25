#include "stdafx.h"
#include "render_object.h"
#include "render_include.h"

#include "dom/dom_include.h"

namespace ui
{
	RenderObject* RenderObject::Create(UIElement* elem)
	{
		const std::string& tag = elem->getTag();
		if (tag == "Window") {
			return new RenderWidget(elem);
		}
		return NULL;
	}

	RenderObject::RenderObject(UINode* node)
		: parent_(NULL)
		, first_(NULL)
		, last_(NULL)
		, prev_(NULL)
		, next_(NULL)
		, node_(node)

	{

	}

	RenderObject::~RenderObject()
	{

	}

	RenderObject* RenderObject::parent() const
	{
		return parent_.get();
	}

	bool RenderObject::isDescendantOf(const RenderObject* obj) const
	{
		RenderObject* p = parent_.get();
		while (p && p != obj)
			p = p->parent();
		return p != NULL;
	}

	UIElement* RenderObject::GetUIElement()
	{
		return dynamic_cast<UIElement*>(node_.get());
	}

	RenderObject* RenderObject::firstChild() const
	{
		return first_.get();
	}

	RenderObject* RenderObject::lastChild() const
	{
		return last_.get();
	}

	RenderObject* RenderObject::previousSibling() const
	{
		return prev_.get();
	}

	RenderObject* RenderObject::nextSibling() const
	{
		return next_.get();
	}

	RenderObject* RenderObject::Append(RenderObject* child)
	{
		return InsertAfter(last_.get(), child);
	}

	RenderObject* RenderObject::Remove(RenderObject* child)
	{
		//只允许移除子节点
		if (!child || child->parent_.get() != this)
		{
			assert(0); //<< "can only remove child node";
			return NULL;
		}

		//更新父子关系
		child->parent_ = NULL;
		if (first_ == child)
		{
			first_ = child->next_;
		}
		if (last_ == child)
		{
			last_ = child->prev_;
		}

		//更新兄弟关系
		if (child->prev_)
		{
			child->prev_->next_ = child->next_;
		}

		if (child->next_)
		{
			child->next_->prev_ = child->prev_;
		}

		child->next_ = NULL;
		child->prev_ = NULL;

		//child_count_--;

		return child;
	}

	RenderObject* RenderObject::InsertAfter(RenderObject* ref, RenderObject* child)
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

		child->prev_ = ref;
		if (ref)
		{//插到中间或最后
			child->next_ = ref->next_;
			if (ref->next_)
			{
				ref->next_->prev_ = child;
			}
			ref->next_ = child;

			assert(last_);
			//插到最后了
			if (last_->next_)
			{
				last_ = last_->next_;
			}
		}
		else
		{//插到最前
			child->next_ = first_;
			if (first_)
			{
				first_->prev_ = child;
			}
			first_ = child;

			//原来没有子元素
			if (last_ == NULL)
			{
				last_ = child;
			}
		}
		//child_count_++;

		return child;
	}

	RenderObject* RenderObject::InsertBefore(RenderObject* ref, RenderObject* child)
	{
		if (ref == child || child == NULL || (ref && ref->parent_ != this))
		{
			assert(0);// << "insert before invalid";
			return NULL;
		}

		if (ref)
		{
			return InsertAfter(ref->prev_.get(), child);
		}
		else
		{
			return InsertAfter(last_.get(), child);
		}
	}

	void RenderObject::SetBounds(int x, int y, int width, int height)
	{
		SetBoundsRect(Rect(x, y, (std::max)(0, width), (std::max)(0, height)));
	}

	void RenderObject::SetBoundsRect(const Rect& bounds)
	{
		if(bounds == bounds_) {
			return;
		}

		Rect prev = bounds_;
		bounds_ = bounds;
		if (prev.size() != size()) {
			Layout();
		}
	}

	void RenderObject::SetSize(const Size& size)
	{
		SetBounds(x(), y(), size.width(), size.height());
	}

	void RenderObject::SetPosition(const Point& position)
	{
		SetBounds(position.x(), position.y(), width(), height());
	}

	void RenderObject::SetX(int x)
	{
		SetBounds(x, y(), width(), height());
	}

	void RenderObject::SetY(int y)
	{
		SetBounds(x(), y, width(), height());
	}

	Rect RenderObject::GetLocalBounds() const
	{
		return Rect(width(), height());
	}

	

}