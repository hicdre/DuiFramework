#include "stdafx.h"
#include "box_layout.h"

#include "core/view.h"

namespace ui
{
	

	BoxLayout::BoxLayout(bool vertical /*= false*/)
		: is_vertical_(vertical)
	{

	}


	BoxLayout::~BoxLayout()
	{

	}

	void BoxLayout::Layout(View* host)
	{
		Rect child_area(host->GetContentsBounds());
		child_area.Inset(padding_);
		child_area.Inset(-child_spaceing_, 0, 0, 0);
		for (View* v = host->first_child(); v != NULL; v = v->next_sibling())
		{
			if (!v->visible())
				continue;

			Size sz(v->GetPreferredSize());
			int x = is_vertical_ ? child_area.x() : child_area.x() + child_spaceing_;
			int y = is_vertical_ ? child_area.y() + child_spaceing_ : child_area.y();
			int w = is_vertical_ ? sz.width() : child_area.width();
			int h = is_vertical_ ? child_area.height() : sz.height();

			Rect child_bounds(x, y, w, h);
			child_area = child_area.Intersect(child_bounds);
			v->SetBoundsRect(child_bounds);
		}
	}

	Size BoxLayout::GetPreferredSize(const View* host) const
	{
		int width = is_vertical_ ? 0 : -child_spaceing_;
		int height = is_vertical_ ? -child_spaceing_ : 0;
		for (View* v = host->first_child(); v != NULL; v = v->next_sibling())
		{
			if (!v->visible())
				continue;

			Size sz(v->GetPreferredSize());

			if (is_vertical_)
			{
				width = (std::max)(width, sz.width());
				height += child_spaceing_ + sz.height();
			}
			else
			{
				height = (std::max)(height, sz.height());
				width += child_spaceing_ + sz.width();
			}
		}

		return Size((std::max)(width, 0), (std::max)(height, 0));
	}

	void BoxLayout::SetVertical(bool v)
	{
		is_vertical_ = v;
	}

	bool BoxLayout::IsVertical() const
	{
		return is_vertical_;
	}

	void BoxLayout::SetChildSpaceing(int i)
	{
		child_spaceing_ = i;
	}

	void BoxLayout::SetPadding(int left, int top, int right, int bottom)
	{
		padding_.SetPadding(left, top, right, bottom);
	}


}