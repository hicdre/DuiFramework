#include "stdafx.h"
#include "cairo_render_font.h"

namespace ui
{
	UICairoRenderFont::UICairoRenderFont(UIFont* font, UICairoRenderEngine* engine)
		: font_(font)
		, engine_(engine)
	{

	}

	UICairoRenderFont::~UICairoRenderFont()
	{
		if (cairoScaledFont_)
		{
			cairo_scaled_font_destroy(cairoScaledFont_);
			cairoScaledFont_ = NULL;
		}

		if (cairoFontExtent_)
		{
			delete cairoFontExtent_;
			cairoFontExtent_ = NULL;
		}
	}


	void UICairoRenderFont::GetGlyphs(const wchar_t* text, size_t len, 
		UIGlyph** glyphs, size_t& glyphsCount)
	{
		std::string utf8_contents = WideToMultiByte(std::wstring(text, len));

		cairo_scaled_font_t* scaledFont = cairoScaledFont();
		//获取字形
		cairo_text_cluster_t *clusters = NULL;
		int numClusters;
		cairo_glyph_t *cairoGlyphs = NULL;
		int numGlyphs;
		cairo_text_cluster_flags_t cluster_flags;
		cairo_scaled_font_text_to_glyphs(scaledFont, 0, 0,
			utf8_contents.c_str(), utf8_contents.size(),
			&cairoGlyphs, &numGlyphs,
			&clusters, &numClusters, &cluster_flags);

		assert(numClusters == numGlyphs);
		glyphsCount = numGlyphs;

		if (glyphs == NULL)
			return;

		UIGlyph* glyphsAdvance = new UIGlyph[glyphsCount];
		for (int i = 0; i < glyphsCount; ++i)
		{
			glyphsAdvance[i].index = cairoGlyphs[i].index;

			cairo_text_extents_t extents;
			cairo_scaled_font_glyph_extents(scaledFont, &cairoGlyphs[i], 1, &extents);
			glyphsAdvance[i].x_advance = extents.x_advance;
			glyphsAdvance[i].y_advance = extents.y_advance;
			glyphsAdvance[i].width = extents.width;
			glyphsAdvance[i].height = extents.height;
			glyphsAdvance[i].x_bearing = extents.x_bearing;
			glyphsAdvance[i].y_bearing = extents.y_bearing;
		}

		cairo_glyph_free(cairoGlyphs);
		cairo_text_cluster_free(clusters);

		*glyphs = glyphsAdvance;
	}

	cairo_scaled_font_t* UICairoRenderFont::cairoScaledFont()
	{
		if (cairoScaledFont_ == NULL)
		{
			cairo_matrix_t fm;
			cairo_matrix_t ctm;
			cairo_font_options_t *op = cairo_font_options_create();

			cairo_matrix_init_identity(&ctm);
			cairo_matrix_init_identity(&fm);
			cairo_matrix_init_scale(&fm, font_->font_size(), font_->font_size());

			cairo_font_face_t* fontFace = engine_->font_manager()->GetFont(font_);
			cairoScaledFont_ = cairo_scaled_font_create(fontFace, &fm, &ctm, op);
		}
		return cairoScaledFont_;
	}

	double UICairoRenderFont::GetHeight() 
	{
		return cairoFontExtent()->height;
	}

	double UICairoRenderFont::GetAscent() 
	{
		return cairoFontExtent()->ascent;
	}

	double UICairoRenderFont::GetDescent() 
	{
		return cairoFontExtent()->descent;
	}

	cairo_font_extents_t* UICairoRenderFont::cairoFontExtent()
	{
		if (cairoFontExtent_ == NULL)
		{
			cairoFontExtent_ = new cairo_font_extents_t;
			cairo_scaled_font_extents(cairoScaledFont(), cairoFontExtent_);
		}
		return cairoFontExtent_;
	}

	

}