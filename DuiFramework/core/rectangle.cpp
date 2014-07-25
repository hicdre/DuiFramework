#include "stdafx.h"
#include "rectangle.h"

#include "render/painter.h"
#include "utils/image_store.h"

namespace ui
{

	SolidRectangle::SolidRectangle()
	{
		set_leaf_view(true);
	}

	SolidRectangle::SolidRectangle(Color color)
		: color_(color)
	{

	}

	void SolidRectangle::SetColor(Color color)
	{
		if (color_ != color)
		{
			color_ = color;
			SchedulePaint();
		}
	}

	void SolidRectangle::OnPaint(Painter* painter)
	{
		painter->FillRect(GetContentsBounds(), color_);
	}

}