#include "stdafx.h"
#include "cairo_text_layout.h"
#include "cairo_render_engine.h"
#include "cairo_render_context.h"

namespace ui
{


	UICairoTextLayout::UICairoTextLayout(const UIFont* font, 
		const Rect& rect,
		UICairoRenderEngine* engine)
		: layoutBounds_(rect)
		, engine_(engine)
		, needLayout_(true)
		, layoutGlyphs_(NULL)
		, layoutGlyphsNum_(0)
	{
		font_.reset(const_cast<UIFont*>(font));
	}


	UICairoTextLayout::~UICairoTextLayout()
	{
		CleanLayoutResult();
	}

	void UICairoTextLayout::SetText(const std::wstring& str)
	{
		if (str == contents_)
			return;
		contents_ = str;
		needLayout_ = true;
	}

	void UICairoTextLayout::SetFont(const UIFont* font)
	{
		if (font->IsEqual(font_))
			return;
		font_.reset(const_cast<UIFont*>(font));
		needLayout_ = true;
	}

	void UICairoTextLayout::SetBounds(const Rect& rect)
	{
		if (layoutBounds_ == rect)
			return;
		layoutBounds_ = rect;
		needLayout_ = true;
	}

	Rect UICairoTextLayout::Layout()
	{
		if (!needLayout_)
			return textBounds_;

		RealLayout();
		needLayout_ = false;
		return textBounds_;
	}

	void UICairoTextLayout::Render(UIRenderContext* context, Color color)
	{
		UICairoRenderContext* cairoContext = dynamic_cast<UICairoRenderContext*>(context);
		if (!context)
			return;
		Layout();

		if (layoutGlyphsNum_ == NULL)// 空字符串
			return;
		cairo_t* cr = cairoContext->get_context();
		cairo_scaled_font_t* sf = InitCairoFont();
		cairoContext->InitColor(color);
		cairo_set_scaled_font(cr, sf);
		cairo_show_glyphs(cr, layoutGlyphs_, layoutGlyphsNum_);
	}

	void UICairoTextLayout::CleanLayoutResult()
	{
		if (layoutGlyphs_)
		{
			cairo_glyph_free(layoutGlyphs_);
			layoutGlyphs_ = NULL;
			layoutGlyphsNum_ = 0;
		}
	}

	void UICairoTextLayout::RealLayout()
	{
		CleanLayoutResult();

		cairo_scaled_font_t* sf = InitCairoFont();

		std::string utf8_contents = WideToMultiByte(contents_);

		//获取整体布局大小
		cairo_text_extents_t extents;
		cairo_scaled_font_text_extents(sf, utf8_contents.c_str(), &extents);

		//获取字形
		cairo_text_cluster_t *clusters = NULL;
		int numClusters;
		cairo_text_cluster_flags_t cluster_flags;
		cairo_scaled_font_text_to_glyphs(sf, 0, extents.height,
			utf8_contents.c_str(), utf8_contents.size(),
			&layoutGlyphs_, &layoutGlyphsNum_,
			&clusters, &numClusters, &cluster_flags);

		//utf8 => utf16

		textBounds_.SetSize(extents.width, font_->font_size());
	}

	cairo_scaled_font_t* UICairoTextLayout::InitCairoFont()
	{
		cairo_font_face_t* fontFace = engine()->font_manager()->GetFont(font_);
		cairo_matrix_t fm;
		cairo_matrix_t ctm;
		cairo_font_options_t *op = cairo_font_options_create();
		cairo_matrix_init_identity(&ctm);
		//cairo_matrix_init_translate(&ctm, 20, 50);

		cairo_matrix_init_identity(&fm);
		cairo_matrix_init_scale(&fm, font_->font_size(), font_->font_size());

		cairo_font_options_set_hint_metrics(op, CAIRO_HINT_METRICS_ON);
		cairo_scaled_font_t* ret = cairo_scaled_font_create(fontFace, &fm, &ctm, op);
		return ret;
	}


// 	void UICairoTextLayout::Build(UIRenderContext* context, const std::wstring& contents)
// 	{
// 		contents_ = contents;
// 		std::string utf8_contents = WideToMultiByte(contents);
// 
// 		cairo_font_face_t* f = engine()->font_manager()->GetFont(font_);
// 		cairo_matrix_t fm;
// 		cairo_matrix_t ctm;
// 		cairo_font_options_t *op = cairo_font_options_create();
// 		cairo_scaled_font_t* sf = cairo_scaled_font_create(f, &fm, &ctm, op);
// 
// 		cairo_text_extents_t extents;
// 		cairo_scaled_font_text_extents(sf, utf8_contents.c_str(), &extents);
// 
// 		cairo_glyph_t *glyphs = NULL;
// 		int num_glyphs;
// 		cairo_text_cluster_t *clusters = NULL;
// 		int num_clusters;
// 		cairo_text_cluster_flags_t cluster_flags;
// 		cairo_scaled_font_text_to_glyphs(sf, 0, extents.height,
// 			utf8_contents.c_str(), utf8_contents.size(),
// 			&glyphs, &num_glyphs,
// 			&clusters, &num_clusters, &cluster_flags);
// 	}

}