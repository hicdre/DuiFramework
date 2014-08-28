#include "stdafx.h"
#include "rectangle.h"

#include "render/painter.h"
#include "utils/image_store.h"

namespace ui
{

	SolidRectangle::SolidRectangle()
	{
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
		}
	}

	void SolidRectangle::DoPaint(Painter* painter, const Rect& dest)
	{
		painter->FillRect(dest, color_);
	}

}