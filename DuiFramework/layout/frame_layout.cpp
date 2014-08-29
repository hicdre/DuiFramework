#include "stdafx.h"
#include "frame_layout.h"

namespace ui
{
	FrameLayout::FrameLayout()
	{

	}

	FrameLayout::~FrameLayout()
	{

	}

	void FrameLayout::Layout()
	{
		Rect child_area(GetContentsBounds());
		for (View* v = first_child(); v != NULL; v = v->next_sibling())
		{
			if (!v->visible())
				continue;

			v->SetBoundsRect(child_area);
		}
	}

	Size FrameLayout::GetPreferredSize() const
	{
		int width = 0;
		int height = 0;
		for (View* v = first_child(); v != NULL; v = v->next_sibling())
		{
			if (!v->visible())
				continue;

			Size sz(v->GetPreferredSize());
			width = (std::max)(width, sz.width());
			height = (std::max)(height, sz.height());
		}

		if (border())
		{
			Padding pd(border()->GetPadding());
			width += pd.left() + pd.right();
			height += pd.top() + pd.bottom();
		}
		return Size(width, height);
	}
}