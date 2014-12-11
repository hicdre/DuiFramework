#include "stdafx.h"
#include "render_context_private.h"

namespace ui
{
	void InitCario()
	{
		cairo_startup();
	}

	void UnintCario()
	{
		cairo_shutdown();
	}
	

	RenderContextPrivate::RenderContextPrivate(Widget* widget)
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

	RenderContextPrivate::~RenderContextPrivate()
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

	void RenderContextPrivate::PushState()
	{
		cairo_save(cairo_);
	}

	void RenderContextPrivate::PopState()
	{
		cairo_restore(cairo_);
	}

	void RenderContextPrivate::FillRect(const Rect& rect, Color color)
	{
		if (color == Color_Transparent)
			return;

		cairo_new_sub_path(cairo_);
		cairo_rectangle(cairo_, rect.x(), rect.y(), rect.width(), rect.height());
		set_color(color);
		cairo_fill(cairo_);
	}


	void RenderContextPrivate::DrawArc(const Rect& rect, int from, int angles, Color color, int width)
	{
		if (color == Color_Transparent)
			return;
	}


	void RenderContextPrivate::Trans(const Matrix& m)
	{
		cairo_matrix_t tm;
		cairo_matrix_init(&tm, m.a, m.b, m.c, m.d, m.tx, m.ty);
		cairo_transform(cairo_, &tm);
	}


	void RenderContextPrivate::set_color(Color color)
	{
		cairo_set_source_rgba(cairo_,
			ColorGetR(color) / 255.0,
			ColorGetG(color) / 255.0,
			ColorGetB(color) / 255.0,
			ColorGetA(color) / 255.0);
	}

	void RenderContextPrivate::NewPath()
	{
		cairo_new_path(cairo_);
	}

	void RenderContextPrivate::ClosePath()
	{
		cairo_close_path(cairo_);
	}

	

	

}