#include "stdafx.h"
#include "fluid_layout.h"

namespace ui
{
	FluidLayout::FluidLayout(bool horizonal)
		: is_horizonal_(horizonal)
	{

	}

	FluidLayout::~FluidLayout()
	{

	}

	void FluidLayout::Layout(View* host)
	{
		Rect child_area(host->GetContentsBounds());

		if (child_area.IsEmpty())
			return;

		if (is_horizonal_)
			LayoutHorizonal(child_area);
		else
			LayoutVerical(child_area);
	}

	void FluidLayout::SetNearView(View* view)
	{
		near_ = view;
	}

	void FluidLayout::SetCenterView(View* view)
	{
		center_ = view;
	}

	void FluidLayout::SetFarView(View* view)
	{
		far_ = view;
	}

	void FluidLayout::LayoutHorizonal(const Rect& child_area)
	{
		int near_width = near_ ? near_->GetPreferredSize().width() : 0;
		int far_width = far_ ? far_->GetPreferredSize().width() : 0;
		int center_width = center_ ? center_->GetPreferredSize().width() : 0;

		if (near_width + far_width > child_area.width())
		{//固定宽度大于父窗口宽度，按比例缩放
			float percent = 1.0 - child_area.width() / (float)(near_width + far_width);
			near_width = near_width * percent;
			far_width = far_width * percent;
			center_width = 0;
		}
		else
		{
			center_width = child_area.width() - near_width - far_width;
		}

		if (near_)
		{
			Rect child_bounds(child_area.x(), child_area.y(), 
				near_width, child_area.height());
			near_->SetBoundsRect(child_bounds);
		}

		if (far_)
		{
			Rect child_bounds(child_area.right() - far_width, child_area.y(), 
				far_width, child_area.height());
			far_->SetBoundsRect(child_bounds);
		}

		if (center_)
		{
			Rect child_bounds(child_area.x() + near_width, child_area.y(),
				center_width, child_area.height());
			center_->SetBoundsRect(child_bounds);
		}
	}

	void FluidLayout::LayoutVerical(const Rect& child_area)
	{
		int near_height = IsInLayout(near_) ? near_->GetPreferredSize().height() : 0;
		int far_height = IsInLayout(far_) ? far_->GetPreferredSize().height() : 0;
		int center_height = IsInLayout(center_) ? center_->GetPreferredSize().height() : 0;

		if (near_height + far_height > child_area.height())
		{//固定宽度大于父窗口宽度，按比例缩放
			float percent = 1.0 - child_area.height() / (float)(near_height + far_height);
			near_height = near_height * percent;
			far_height = far_height * percent;
			center_height = 0;
		}
		else
		{
			center_height = child_area.height() - near_height - far_height;
		}

		if (IsInLayout(near_))
		{
			Rect child_bounds(child_area.x(), child_area.y(),
				child_area.width(), near_height);
			near_->SetBoundsRect(child_bounds);
		}

		if (IsInLayout(far_))
		{
			Rect child_bounds(child_area.x(), child_area.bottom() - far_height,
				child_area.width(), far_height);
			far_->SetBoundsRect(child_bounds);
		}

		if (IsInLayout(center_))
		{
			Rect child_bounds(child_area.x(), child_area.y() + near_height,
				child_area.width(), center_height);
			center_->SetBoundsRect(child_bounds);
		}
	}

	Size FluidLayout::GetPreferredSize(const View* host) const
	{
		Size near_size = IsInLayout(near_) ? near_->GetPreferredSize() : Size();
		Size far_size = IsInLayout(far_) ? far_->GetPreferredSize() : Size();
		Size center_size = IsInLayout(center_) ? center_->GetPreferredSize() : Size();

		int width, height;
		if (is_horizonal_)
		{
			height = (std::max)((std::max)(near_size.height(), far_size.height()), center_size.height());
			width = near_size.width() + far_size.width() + center_size.width();
		}
		else
		{
			width = (std::max)((std::max)(near_size.width(), far_size.width()), center_size.width());
			height = near_size.height() + far_size.height() + center_size.height();
		}

		return Size(width, height);
	}

	bool FluidLayout::IsInLayout(View* v) const
	{
		return v && v->visible();
	}


}