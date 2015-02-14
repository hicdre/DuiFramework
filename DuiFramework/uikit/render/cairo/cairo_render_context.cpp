#include "stdafx.h"
#include "cairo_render_context.h"
#include "render_path_cairo.h"
#include "cairo_render_engine.h"
#include "cairo_render_font.h"

//#include "render_image_cairo.h"
// #include "framework/widget.h"
// #include "utils/utils.h"

namespace ui
{

	UICairoRenderContext::UICairoRenderContext(HWND hwnd, UICairoRenderEngine* engine)
 		: hwnd_(hwnd)
		, cairo_(NULL)
		, surface_(NULL)
		, engine_(engine)
	{
		BeginPaint();
	}

	UICairoRenderContext::~UICairoRenderContext()
	{
		EndPaint();
	}


// 	scoped_refptr<UIRenderPath> UICairoRenderContext::OpenRenderPath()
// 	{
// 		return new UIRenderPathCairo(this);
// 	}


	

	void UICairoRenderContext::StorkeRect(const Rect& rect, Color color, uint32 size)
	{
		if (color == Color_Transparent)
			return;

		cairo_new_path(cairo_);
		cairo_rectangle(cairo_, rect.x(), rect.y(), rect.width(), rect.height());

		InitColor(color);
		cairo_set_line_width(cairo_, size);
		cairo_stroke_preserve(cairo_);
	}

	void UICairoRenderContext::StorkePath(const UIRenderPath* path, Color color, uint32 size)
	{
		if (color == Color_Transparent)
			return;

		PeparePath(path);

		InitColor(color);
		cairo_set_line_width(cairo_, size);
		cairo_stroke_preserve(cairo_);
	}

	void UICairoRenderContext::FillRect(const Rect& rect, Color color)
	{
		if (color == Color_Transparent)
			return;

		cairo_new_path(cairo_);
		cairo_rectangle(cairo_, rect.x(), rect.y(), rect.width(), rect.height());
		InitColor(color);
		cairo_fill_preserve(cairo_);
	}


	void UICairoRenderContext::FillPath(const UIRenderPath* path, Color color)
	{
		if (color == Color_Transparent)
			return;

		PeparePath(path);

		InitColor(color);
		cairo_fill_preserve(cairo_);//保留当前路径
	}

	void UICairoRenderContext::Trans(const Matrix& m)
	{
		cairo_matrix_t tm;
		cairo_matrix_init(&tm, m.a, m.b, m.c, m.d, m.tx, m.ty);
		cairo_transform(cairo_, &tm);
	}

	void UICairoRenderContext::PushState()
	{
		cairo_save(cairo_);
	}

	void UICairoRenderContext::PopState()
	{
		cairo_restore(cairo_);
	}


	void UICairoRenderContext::InitColor(Color color)
	{
		cairo_set_source_rgba(cairo_,
			ColorGetR(color) / 255.0,
			ColorGetG(color) / 255.0,
			ColorGetB(color) / 255.0,
			ColorGetA(color) / 255.0);
	}

	void UICairoRenderContext::NewPath()
	{
		cairo_new_path(cairo_);
	}

	void UICairoRenderContext::PeparePath(const UIRenderPath* path)
	{
// 		UIRenderPathCairo* cairo_path = const_cast<UIRenderPathCairo*>(static_cast<const UIRenderPathCairo*>(path));
// 		cairo_path_t* detach_path = cairo_path->get_path();
// 		cairo_new_path(cairo_);
// 		cairo_append_path(cairo_, detach_path);
	}

	void UICairoRenderContext::ClipRect(const Rect& rect)
	{
		cairo_new_path(cairo_);
		cairo_rectangle(cairo_, rect.x(), rect.y(), rect.width(), rect.height());

		cairo_clip_preserve(cairo_);
	}

	void UICairoRenderContext::ClipPath(const UIRenderPath* path)
	{
		PeparePath(path);

		cairo_clip_preserve(cairo_);
	}

// 	void RenderTargetCairo::DrawImage(const RenderImage *image, const Rect& dest)
// 	{
// 		RenderImageCairo* image_cairo = const_cast<RenderImageCairo*>(static_cast<const RenderImageCairo*>(image));
// 		
// 		Size src = image_cairo->GetSize();
// 
// 		//计算矩阵，变换等
// 		cairo_save(cairo_);
// 		{
// 			cairo_translate(cairo_, dest.x(), dest.y());
// 			cairo_scale(cairo_
// 				, (float)dest.width() / (float)src.width()
// 				, (float)dest.height() / (float)src.height());
// 
// 			cairo_set_source_surface(cairo_, image_cairo->GetSurface(), 0, 0);
// 			cairo_paint(cairo_);
// 		}
// 		cairo_restore(cairo_);
// 	}
// 
// 	void RenderTargetCairo::DrawImage(const RenderImage *image, const Rect& src, const Rect& dest)
// 	{
// 		RenderImageCairo* image_cairo = const_cast<RenderImageCairo*>(static_cast<const RenderImageCairo*>(image));
// 		//计算矩阵，变换等
// 		cairo_save(cairo_);
// 		{
//  			cairo_translate(cairo_, dest.x(), dest.y());
//  			cairo_scale(cairo_
//  				, (float)dest.width() / (float)src.width()
//  				, (float)dest.height() / (float)src.height());
// 
// 			cairo_set_source_surface(cairo_, image_cairo->GetSurface(), -src.x(), -src.y());
// 			cairo_paint(cairo_);
// 		}
// 		cairo_restore(cairo_);
// 	}

	void UICairoRenderContext::ShowGlyphs(const UIGlyph* glyphs, size_t glyphsCount, UIRenderFont* font, Color color)
	{
		InitColor(color);

		UICairoRenderFont* cairoFont = static_cast<UICairoRenderFont*>(font);
		cairo_set_scaled_font(cairo_, cairoFont->cairoScaledFont());

		double x_advance = -glyphs[0].x_bearing;
		double y_advance = font->GetAscent();

		cairo_glyph_t glyph;
		for (int i = 0; i < glyphsCount; ++i)
		{
			glyph.index = glyphs[i].index;
			glyph.x = x_advance;
			glyph.y = y_advance;
			x_advance += glyphs[i].x_advance;
			y_advance += glyphs[i].y_advance;
			cairo_show_glyphs(cairo_, &glyph, 1);
		}
	}


// 	void UICairoRenderContext::ShowGlyphs(const UIGlyphBuffer& buffer, const UIFont* font, Color color)
// 	{
// 		std::vector<cairo_glyph_t> glyphs(buffer.count);
// 		for (uint32 i = 0; i < buffer.count; ++i) {
// 			glyphs[i].index = buffer.glyphs[i].index;
// 			glyphs[i].x = buffer.glyphs[i].pos.x();
// 			glyphs[i].y = buffer.glyphs[i].pos.y();
// 		}
// 		InitColor(color);
// 
// //		cairo_font_face_t* f;//create cairo font
// 		//cairo_font_face_t* f = cairo_win32_font_face_create_for_hfont(font.ToHFONT());
// 
// // 		cairo_set_font_face(cairo_, f);
// // 		cairo_set_font_size(cairo_, font->font_size());
// // 		cairo_show_glyphs(cairo_, &glyphs[0], buffer.count);
// // 
// // 		cairo_font_face_destroy(f);
// 	}

	void UICairoRenderContext::BeginPaint()
	{
		::BeginPaint(hwnd_, &ps_);

		RECT rc;
		::GetClientRect(hwnd_, &rc);
		rect_.SetRect(rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top);

		surface_ = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, rect_.width(), rect_.height());
		cairo_ = cairo_create(surface_);
	}

	void UICairoRenderContext::EndPaint()
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
				
		::EndPaint(hwnd_, &ps_);
	}

#define CBU8_LENGTH(c) \
    ((uint32)(c)<=0x7f ? 1 : \
        ((uint32)(c)<=0x7ff ? 2 : \
            ((uint32)(c)<=0xd7ff ? 3 : \
                ((uint32)(c)<=0xdfff || (uint32)(c)>0x10ffff ? 0 : \
                    ((uint32)(c)<=0xffff ? 3 : 4)\
                ) \
            ) \
        ) \
    )

	void UICairoRenderContext::DrawText(const std::wstring& text, const UIFont* font, Color color)
	{
		InitColor(color);

		cairo_font_face_t* f = engine()->font_manager()->GetFont(font);
		cairo_set_font_face(cairo_, f);
		cairo_set_font_size(cairo_, font->font_size());

		

		std::string utf8_str = WideToMultiByte(text);

		cairo_text_extents_t extents;
		cairo_glyph_t *glyphs = NULL;
		int num_glyphs;
		cairo_text_cluster_t *clusters = NULL;
		int num_clusters;
		cairo_text_cluster_flags_t cluster_flags;

		cairo_scaled_font_t* sf = cairo_get_scaled_font(cairo_);
		cairo_scaled_font_text_extents(sf, utf8_str.c_str(), &extents);

		cairo_scaled_font_text_to_glyphs(sf, 0, extents.height,
			utf8_str.c_str(), utf8_str.size(),
			&glyphs, &num_glyphs,
			&clusters, &num_clusters, &cluster_flags);


		cairo_show_glyphs(cairo_, glyphs, num_glyphs);
// 		cairo_show_text_glyphs(cairo_,
// 			utf8_str.c_str(), utf8_str.size(),
// 			glyphs, num_glyphs,
// 			clusters, num_clusters, cluster_flags);

		cairo_glyph_free(glyphs);
		cairo_text_cluster_free(clusters);
	}

	void UICairoRenderContext::Translate(double dx, double dy)
	{
		cairo_translate(cairo_, dx, dy);
	}

// 	void RenderTargetCairo::DrawText(const std::wstring& buffer, const UIFont& font, Color color)
// 	{
// 		cairo_font_face_t* f = cairo_win32_font_face_create_for_hfont(font.ToHFONT());
// 
// 		cairo_save(cairo_);
// 		cairo_new_path(cairo_);
// 
// 		cairo_move_to(cairo_, 10, 10);
// 
// 		cairo_set_font_face(cairo_, f);
// 		cairo_set_font_size(cairo_, font.GetFontSize());
// 		cairo_show_text(cairo_, WideToMultiByte(buffer).c_str());
// 
// 		cairo_restore(cairo_);
// 		cairo_font_face_destroy(f);
// 	}


	


	

	

}