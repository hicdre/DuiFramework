#include "stdafx.h"
#include "view_container.h"
#include "view.h"

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
		views_.push_back(v);
	}

	void SequenceContainer::RemoveChild(View* v)
	{
		for (auto iter = views_.begin(); iter != views_.end(); iter++)
		{
			if (*iter = v)
			{
				views_.erase(iter);
				break;
			}
		}
	}

	View* SequenceContainer::GetView(int index)
	{
		return views_.at(index);
	}


	void AbsoulteContainer::Layout()
	{
		for (View* view : views_)
		{
			if (view->visible())
			{
				LayoutData* layout = view->layout();
				int x = layout->x();
				int y = layout->y();
				int w = layout->width();
				int h = layout->height();
				view->SetBounds(x, y, w, h);
			}
		}
	}

}