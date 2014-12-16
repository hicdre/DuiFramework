#include "stdafx.h"
#include "view_container.h"
#include "view.h"
#include "render/render_context.h"

namespace ui
{


	Container::Container()
		: parent_(NULL)
	{

	}

	Container::Container(View* parent)
		: parent_(parent)
	{

	}

	Container::~Container()
	{

	}

	SequenceContainer::SequenceContainer()
		: Container()
	{

	}

	SequenceContainer::SequenceContainer(View* parent)
		: Container(parent)
	{

	}

	int32 SequenceContainer::GetChildCount() const
	{
		return views_.size();
	}

	void SequenceContainer::AddChild(View* v)
	{
		if (views_.size())
		{
			View* vi = *views_.rbegin();
			vi->setNextSibling(v);
			v->setPrevSibling(vi);
		}
		v->setParent(parent_);
		
		views_.push_back(v);
	}

	void SequenceContainer::RemoveChild(View* v)
	{
		for (auto iter = views_.begin(); iter != views_.end(); iter++)
		{
			if (*iter == v)
			{
				v->setParent(NULL);
				View* next = v->nextSibling();
				View* prev = v->prevSibling();
				if (prev)
					prev->setNextSibling(next);
				if (next)
					next->setPrevSibling(prev);
				views_.erase(iter);
				break;
			}
		}
	}

	View* SequenceContainer::GetView(int index)
	{
		return views_.at(index);
	}


	void SequenceContainer::DoPaint(RenderContext* painter, const Rect& dest)
	{
		ScopedPainter helper(painter, Matrix(1.0, 0, 0, 1.0, dest.x(), dest.y()));

		for (View* view : views_)
		{
			if (view->visible())
			{
				view->DoPaint(painter, view->bounds());
			}
		}
	}

	void SequenceContainer::UpdateStyleRule(StyleSheetList* sheet)
	{
		for (View* view : views_)
		{
			view->UpdateStyleRules(sheet);
		}
	}



	void AbsoulteContainer::Layout()
	{
		Rect bounds(parent_->bounds());
		for (View* view : views_)
		{
			if (view->visible())
			{
				int x = view->layoutX();
				int y = view->layoutY();
				int w = view->layoutWidth();
				int h = view->layoutHeight();
				view->SetBounds(x, y, w, h);
			}
		}
	}

	AbsoulteContainer::AbsoulteContainer()
		: SequenceContainer()
	{

	}

	AbsoulteContainer::AbsoulteContainer(View* parent)
		: SequenceContainer(parent)
	{

	}

	int32 AbsoulteContainer::GetAutoWidth()
	{
		int width = 0;
		for (View* view : views_)
		{
			if (view->visible())
			{
				width += view->layoutWidth();
			}
		}
		return width;
	}

	int32 AbsoulteContainer::GetAutoHeight()
	{
		int height = 0;
		for (View* view : views_)
		{
			if (view->visible())
			{
				height += view->layoutHeight();
			}
		}
		return height;
	}

	ui::LayoutType AbsoulteContainer::layoutType() const
	{
		return AbsoulteLayout;
	}


	HorizonalContainer::HorizonalContainer()
		: SequenceContainer()
	{

	}

	HorizonalContainer::HorizonalContainer(View* parent)
		: SequenceContainer(parent)
	{

	}

	void HorizonalContainer::Layout()
	{
		//Rect bounds(parent_->bounds());
		for (View* view : views_)
		{
			if (view->visible())
			{
				int x = view->layoutX();
				int y = view->layoutY();
				int w = view->layoutWidth();
				int h = view->layoutHeight();
				view->SetBounds(x, y, w, h);
			}
		}
	}

	int32 HorizonalContainer::GetAutoWidth()
	{
		int width = 0;
		for (View* view : views_)
		{
			if (view->visible())
			{
				int new_width = view->layoutX() + view->layoutWidth() + view->marginRight();
				if (new_width > width)
					width = new_width;
			}
		}
		return width;
	}

	int32 HorizonalContainer::GetAutoHeight()
	{
		int height = 0;
		for (View* view : views_)
		{
			if (view->visible())
			{
				int new_height = view->marginTop() + view->layoutHeight() + view->marginBottom();
				if (height < new_height)
					height = new_height;
			}
		}
		return height;
	}

	ui::LayoutType HorizonalContainer::layoutType() const
	{
		return HorizonalLayout;
	}


	VerticalContainer::VerticalContainer()
		: SequenceContainer()
	{

	}

	VerticalContainer::VerticalContainer(View* parent)
		: SequenceContainer(parent)
	{

	}

	ui::LayoutType VerticalContainer::layoutType() const
	{
		return VerticalLayout;
	}

	void VerticalContainer::Layout()
	{
		for (View* view : views_)
		{
			if (view->visible())
			{
				int x = view->layoutX();
				int y = view->layoutY();
				int w = view->layoutWidth();
				int h = view->layoutHeight();
				view->SetBounds(x, y, w, h);
			}
		}
	}

	int32 VerticalContainer::GetAutoWidth()
	{
		int width = 0;
		for (View* view : views_)
		{
			if (view->visible())
			{
				int new_width = view->layoutWidth() + view->marginLeft() + view->marginRight();
				if (width < new_width)
					width = new_width;
			}
		}
		return width;
	}

	int32 VerticalContainer::GetAutoHeight()
	{
		int height = 0;
		for (View* view : views_)
		{
			if (view->visible())
			{
				int new_height = view->layoutY() + view->layoutHeight() + view->marginBottom();
				if (new_height > height)
					height = new_height;
			}
		}
		return height;
	}

}