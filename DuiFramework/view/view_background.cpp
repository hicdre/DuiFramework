#include "stdafx.h"
#include "view_background.h"

#include "render/render_context.h"
#include "render/image.h"

namespace ui
{


	Background::Background()
		: color_(Color_White)
		, image_(NULL)
		, is_size_set_(false)
	{

	}

	Background::~Background()
	{
	}

	void Background::SetColor(Color color)
	{
		color_ = color;
	}

	void Background::SetImage(Image* image)
	{
		image_.reset(image);
	}

	void Background::SetPosition(const Point& pt)
	{
		rect_.set_origin(pt);
	}

	void Background::SetSize(const Size& sz)
	{
		is_size_set_ = true;
		rect_.SetRect(rect_.x(), rect_.y(), sz.width(), sz.height());
	}


	void Background::RestSize()
	{
		is_size_set_ = false;
	}


	void Background::DoPaint(RenderContext* painter, const Rect& dest)
	{
		ScopedPainter s(painter, Matrix(1.0, 0, 0, 1.0, dest.x(), dest.y()));
		
		Rect rc(rect_.x(), rect_.y(),
			is_size_set_ ? rect_.width() : dest.width(),
			is_size_set_ ? rect_.height() : dest.height());
		//ScopedClipper clipper(painter, rc);
		painter->FillRect(rc, color_);
		if (image_.get())
		{
			image_->DoPaint(painter, rc);
		}
	}

	

}