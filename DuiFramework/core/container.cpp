#include "stdafx.h"
#include "container.h"

namespace ui
{


	Container::Container()
	{

	}

	Container::~Container()
	{

	}

	View* Container::first_child() const
	{
		return first_child_;
	}

	View* Container::last_child() const
	{
		return last_child_;
	}

	void Container::GetViews(Views &child_array) const
	{
		for (View* p = first_child_; p != NULL; p = p->next_sibling())
		{
			child_array.push_back(p);
		}
	}

	int32 Container::GetViewCount() const
	{
		return child_count_;
	}

	View* Container::Append(View* child)
	{
		return InsertAfter(last_child_, child);
	}

	View* Container::Remove(View* child)
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

		child_count_--;

		return child;
	}

	View* Container::InsertAfter(View* ref, View* child)
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
		child_count_++;

		return child;
	}

	View* Container::InsertBefore(View* ref, View* child)
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

	bool Container::HasDescender(View* descender, int* step /*= NULL*/)
	{
		*step = 0;
		for (View* v = descender; v; v = v->parent(), (*step)++)
		{
			if (v == this)
				return true;
		}
		return false;
	}

	View* Container::Hittest(const Point& pt)
	{
		if (!enable_mouse_event_)
			return NULL;

		//hittest self
		if (!GetLocalBounds().Contains(pt))
			return NULL;

		for (View* p = last_child_; p != NULL; p = p->prev_sibling())
		{
			if (p)
			{
				Point pt_in_child = p->GetTransform().Invert().Apply(pt);
				View* v = p->Hittest(pt_in_child);
				if (v)
					return v;
			}
		}

		return this;
	}

	bool Container::Hittest(const Point& pt, Views& views)
	{
		if (!enable_mouse_event_)
			return NULL;

		if (!GetLocalBounds().Contains(pt))
			return false;

		bool child_hittested = false;
		for (View* p = last_child_; p != NULL; p = p->prev_sibling())
		{
			if (p)
			{
				child_hittested = Hittest(pt, views) || child_hittested;
			}
		}

		if (!child_hittested)
			views.push_back(this);

		return true;
	}


	void Container::Layout()
	{
		for (View* child = first_child(); child != NULL; child = child->next_sibling()) {
			if (child->needs_layout_) {
				child->needs_layout_ = false;
				child->Layout();
			}
		}
	}

	void Container::DoPaint(Painter* painter, const Rect& dest)
	{
		if (!visible_)
			return;

		ScopedPainter helper(painter, Transform(1.0, 0, 0, 1.0, dest.x(), dest.y()));
		DoPaintSelf(painter);

		DoPaintChildren(painter);
	}

	void Container::DoPaintChildren(Painter* painter)
	{
		for (View* p = first_child_; p != NULL; p = p->next_sibling())
		{
			if (p && p->visible())
			{
				p->DoPaint(painter, p->bounds());
			}
		}
	}
}
