#include "stdafx.h"
#include "render_target_cairo.h"
#include "render_path_cairo.h"
#include "render_image_cairo.h"
#include "framework/widget.h"
#include "utils/utils.h"

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
		cairo_stroke_preserve(cairo_);
	}

	void RenderTargetCairo::StorkePath(const RenderPath* path, Color color, uint32 size)
	{
		if (color == Color_Transparent)
			return;

		PeparePath(path);

		set_color(color);
		cairo_set_line_width(cairo_, size);
		cairo_stroke_preserve(cairo_);
	}

	void RenderTargetCairo::FillRect(const Rect& rect, Color color)
	{
		if (color == Color_Transparent)
			return;

		cairo_new_path(cairo_);
		cairo_rectangle(cairo_, rect.x(), rect.y(), rect.width(), rect.height());
		set_color(color);
		cairo_fill_preserve(cairo_);
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
		cairo_new_path(cairo_);
		cairo_append_path(cairo_, detach_path);
	}

	void RenderTargetCairo::ClipRect(const Rect& rect)
	{
		cairo_new_path(cairo_);
		cairo_rectangle(cairo_, rect.x(), rect.y(), rect.width(), rect.height());

		cairo_clip_preserve(cairo_);
	}

	void RenderTargetCairo::ClipPath(const RenderPath* path)
	{
		PeparePath(path);

		cairo_clip_preserve(cairo_);
	}

	void RenderTargetCairo::DrawImage(const RenderImage *image, const Rect& dest)
	{
		RenderImageCairo* image_cairo = const_cast<RenderImageCairo*>(static_cast<const RenderImageCairo*>(image));
		
		Size src = image_cairo->GetSize();

		//计算矩阵，变换等
		cairo_save(cairo_);
		{
			cairo_translate(cairo_, dest.x(), dest.y());
			cairo_scale(cairo_
				, (float)dest.width() / (float)src.width()
				, (float)dest.height() / (float)src.height());

			cairo_set_source_surface(cairo_, image_cairo->GetSurface(), 0, 0);
			cairo_paint(cairo_);
		}
		cairo_restore(cairo_);
	}

	void RenderTargetCairo::DrawImage(const RenderImage *image, const Rect& src, const Rect& dest)
	{
		RenderImageCairo* image_cairo = const_cast<RenderImageCairo*>(static_cast<const RenderImageCairo*>(image));
		//计算矩阵，变换等
		cairo_save(cairo_);
		{
 			cairo_translate(cairo_, dest.x(), dest.y());
 			cairo_scale(cairo_
 				, (float)dest.width() / (float)src.width()
 				, (float)dest.height() / (float)src.height());

			cairo_set_source_surface(cairo_, image_cairo->GetSurface(), -src.x(), -src.y());
			cairo_paint(cairo_);
		}
		cairo_restore(cairo_);
	}

	void RenderTargetCairo::ShowGlyphs(const GlyphBuffer& buffer, const Font& font, Color color)
	{
		std::vector<cairo_glyph_t> glyphs(buffer.count);
		for (uint32 i = 0; i < buffer.count; ++i) {
			glyphs[i].index = buffer.glyphs[i].index;
			glyphs[i].x = buffer.glyphs[i].pos.x();
			glyphs[i].y = buffer.glyphs[i].pos.y();
		}
		set_color(color);

		cairo_font_face_t* f = cairo_win32_font_face_create_for_hfont(font.ToHFONT());

		cairo_set_font_face(cairo_, f);
		cairo_set_font_size(cairo_, font.GetFontSize());
		cairo_show_glyphs(cairo_, &glyphs[0], buffer.count);

		cairo_font_face_destroy(f);
	}

	void RenderTargetCairo::DrawText(const std::wstring& buffer, const Font& font, Color color)
	{
		cairo_font_face_t* f = cairo_win32_font_face_create_for_hfont(font.ToHFONT());

		cairo_save(cairo_);
		cairo_new_path(cairo_);

		cairo_move_to(cairo_, 10, 10);

		cairo_set_font_face(cairo_, f);
		cairo_set_font_size(cairo_, font.GetFontSize());
		cairo_show_text(cairo_, WideToMultiByte(buffer).c_str());

		cairo_restore(cairo_);
		cairo_font_face_destroy(f);
	}


	


	

	

}