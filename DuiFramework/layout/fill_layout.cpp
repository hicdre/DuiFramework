#include "stdafx.h"
#include "fill_layout.h"
#include "core/view.h"

namespace ui
{


	FillLayout::FillLayout()
	{

	}

	FillLayout::~FillLayout()
	{

	}

	void FillLayout::Layout(View* host)
	{
		for (View* p = host->first_child(); p != NULL; p = p->next_sibling())
		{
			p->SetBoundsRect(host->GetContentsBounds());
		}
	}

	Size FillLayout::GetPreferredSize(const View* host) const
	{
		int max_width = 0;
		int max_height = 0;
		for (View* p = host->first_child(); p != NULL; p = p->next_sibling())
		{
			Size sz(p->GetPreferredSize());
			if (sz.width() > max_width)
				max_width = sz.width();
			if (sz.height() > max_height)
				max_height = sz.height();
		}
		return Size(max_width, max_height);
	}

}