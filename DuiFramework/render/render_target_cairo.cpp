#include "stdafx.h"
#include "render_target_cairo.h"
#include "render_path_cairo.h"
#include "core/widget.h"

namespace ui
{

	RenderTargetCairo::RenderTargetCairo(Widget* widget)
		: widget_(widget)
		, cairo_(NULL)
		, surface_(NULL)
	{
		HDC wnd_dc = BeginPaint(widget->hwnd(), &ps_);

		RECT rc;
		::GetClientRect(widget->hwnd(), &rc);
		rect_ = rc;

		surface_ =	cairo_image_surface_create(CAIRO_FORMAT_ARGB32,	rect_.width(), rect_.height());

		cairo_ = cairo_create(surface_);
	}


	scoped_refptr<RenderPath> RenderTargetCairo::OpenRenderPath()
	{
		return new RenderPathCairo(this);
	}


	RenderTargetCairo::~RenderTargetCairo()
	{
		BITMAPINFO bmi = { 0 };
		bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bmi.bmiHeader.biWidth = rect_.width();
		bmi.bmiHeader.biHeight = - rect_.height();
		bmi.bmiHeader.biPlanes = 1;
		bmi.bmiHeader.biBitCount = 32;
		bmi.bmiHeader.biCompression = BI_RGB;
		bmi.bmiHeader.biSizeImage = rect_.width() * rect_.height() * 4;

		StretchDIBits(ps_.hdc,
			// destination rectangle 
			0, 0, rect_.width(), rect_.height(),
			// source rectangle 
			0, 0, rect_.width(), rect_.height(),
			cairo_image_surface_get_data(surface_),
			&bmi,
			DIB_RGB_COLORS,
			SRCCOPY);

		cairo_destroy(cairo_);
		cairo_surface_destroy(surface_);

		EndPaint(widget_->hwnd(), &ps_);
	}
	

	void RenderTargetCairo::StorkeRect(const Rect& rect, Color color, uint32 size)
	{
		if (color == Color_Transparent)
			return;

		cairo_new_path(cairo_);
		cairo_rectangle(cairo_, rect.x(), rect.y(), rect.width(), rect.height());

		set_color(color);
		cairo_set_line_width(cairo_, size);
		cairo_stroke(cairo_);
	}

	void RenderTargetCairo::StorkePath(const RenderPath* path, Color color, uint32 size)
	{
		if (color == Color_Transparent)
			return;

		PeparePath(path);

		set_color(color);
		cairo_set_line_width(cairo_, size);
		cairo_stroke(cairo_);
	}

	void RenderTargetCairo::FillRect(const Rect& rect, Color color)
	{
		if (color == Color_Transparent)
			return;

		cairo_new_path(cairo_);
		cairo_rectangle(cairo_, rect.x(), rect.y(), rect.width(), rect.height());
		set_color(color);
		cairo_fill(cairo_);
	}


	void RenderTargetCairo::FillPath(const RenderPath* path, Color color)
	{
		if (color == Color_Transparent)
			return;

		PeparePath(path);

		set_color(color);
		cairo_fill_preserve(cairo_);//保留当前路径
	}

	void RenderTargetCairo::Trans(const Matrix& m)
	{
		cairo_matrix_t tm;
		cairo_matrix_init(&tm, m.a, m.b, m.c, m.d, m.tx, m.ty);
		cairo_transform(cairo_, &tm);
	}

	void RenderTargetCairo::PushState()
	{
		cairo_save(cairo_);
	}

	void RenderTargetCairo::PopState()
	{
		cairo_restore(cairo_);
	}


	void RenderTargetCairo::set_color(Color color)
	{
		cairo_set_source_rgba(cairo_,
			ColorGetR(color) / 255.0,
			ColorGetG(color) / 255.0,
			ColorGetB(color) / 255.0,
			ColorGetA(color) / 255.0);
	}

	void RenderTargetCairo::NewPath()
	{
		cairo_new_path(cairo_);
	}

	void RenderTargetCairo::PeparePath(const RenderPath* path)
	{
		RenderPathCairo* cairo_path = const_cast<RenderPathCairo*>(static_cast<const RenderPathCairo*>(path));
		cairo_path_t* detach_path = cairo_path->get_path();
		if (detach_path) {
			cairo_append_path(cairo_, detach_path);
		}
	}


	


	

	

}