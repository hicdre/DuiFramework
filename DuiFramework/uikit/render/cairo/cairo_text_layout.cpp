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

		if (layoudAdvance_ == NULL)// 空字符串
			return;
		cairo_t* cr = cairoContext->get_context();
		cairo_scaled_font_t* sf = InitCairoFont();
		cairoContext->InitColor(color);
		cairo_set_scaled_font(cr, sf);

		cairo_save(cr);
		textPagraph_->Render(cr);
		cairo_restore(cr);
	}

	void UICairoTextLayout::CleanLayoutResult()
	{
		if (textPagraph_)
		{
			delete textPagraph_;
			textPagraph_ = NULL;
		}

		if (layoudAdvance_) {
			delete[] layoudAdvance_;
			layoudAdvance_ = NULL;
		}
	}

	void UICairoTextLayout::RealLayout()
	{
		CleanLayoutResult();

		cairo_scaled_font_t* sf = InitCairoFont();
		cairo_font_extents_t fontExtents;
		cairo_scaled_font_extents(sf, &fontExtents);

		{
			std::string utf8_contents = WideToMultiByte(contents_);

			//获取字形
			cairo_text_cluster_t *clusters = NULL;
			int numClusters;
			cairo_glyph_t *layoutGlyphs = NULL;
			int layoutGlyphsNum;
			cairo_text_cluster_flags_t cluster_flags;
			cairo_scaled_font_text_to_glyphs(sf, 0, 0,
				utf8_contents.c_str(), utf8_contents.size(),
				&layoutGlyphs, &layoutGlyphsNum,
				&clusters, &numClusters, &cluster_flags);


			assert(numClusters == layoutGlyphsNum);
			glyphsCount_ = layoutGlyphsNum;

			layoudAdvance_ = new cairo_glyph_advance_t[layoutGlyphsNum];
			for (int i = 0; i < layoutGlyphsNum; ++i)
			{
				layoudAdvance_[i].index = layoutGlyphs[i].index;

				cairo_text_extents_t extents;
				cairo_scaled_font_glyph_extents(sf, &layoutGlyphs[i], 1, &extents);
				layoudAdvance_[i].x_advance = extents.x_advance;
				layoudAdvance_[i].y_advance = extents.y_advance;
				layoudAdvance_[i].width = extents.width;
				layoudAdvance_[i].height = extents.height;
				layoudAdvance_[i].x_bearing = extents.x_bearing;
				layoudAdvance_[i].y_bearing = extents.y_bearing;
			}

			cairo_glyph_free(layoutGlyphs);
			cairo_text_cluster_free(clusters);
		}

		textPagraph_ = new UICairoTextPagraph;
		cairo_glyph_advance_t* layoutAdvance = layoudAdvance_;
		const wchar_t* buffer = contents_.c_str();
		size_t advanceLen = glyphsCount_;
		while (advanceLen)
		{
			UICairoTextRun* textRun = new UICairoTextRun(layoutAdvance, buffer, advanceLen, fontExtents);
			//size_t glyphSize = textRun->layoutGlyphSizeForWidth(layoutBounds_.width());
			size_t glyphSize = textRun->layoutWordSizeForWidth(layoutBounds_.width());
			if (glyphSize < textRun->glyphsCount()) {
				textRun->setGlyphsCount(glyphSize);
				layoutAdvance += glyphSize;
				buffer += glyphSize;
			}

			advanceLen -= glyphSize;
			textPagraph_->addTextRun(textRun);
		}

		//utf8 => utf16

		Rect rc(textPagraph_->GetBoundsRect());

		if (textPagraph_->isMutilLine()) {
			textBounds_.SetSize(layoutBounds_.width(), rc.height());
		}
		else {
			textBounds_.SetSize(rc.width(), rc.height());
		}
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

	static bool isSpace(wchar_t ch) {
		return ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n';
	}

	static size_t nextWord(const wchar_t* buffer, size_t from, size_t end, bool& isSpaceWord)
	{
		isSpaceWord = isSpace(buffer[from]);
		for (size_t pos = from + 1; pos < end; ++pos)
		{
			bool b = isSpace(buffer[pos]);
			if (b != isSpaceWord)
			{
				return pos - from;
			}
		}
		return end - from;
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



	size_t UICairoTextRun::layoutGlyphSizeForWidth(int width)
	{
		size_t w = 0;
		size_t i = 0;
		while (i < glyphsCount_)
		{
			w += glyphs_[i].x_advance;
			if (w > width)
				break;
			++i;
		}
		return i;
	}

	size_t UICairoTextRun::layoutWordSizeForWidth(int width)
	{
		size_t lastCount = glyphsCount_;
		size_t w = 0;
		size_t pos = 0;
		while (pos < glyphsCount_)
		{
			bool isSpaceWord = false;
			size_t wordCount = nextWord(buffer_, pos, glyphsCount_, isSpaceWord);

			size_t wordWidth = 0;
			for (int i = 0; i < wordCount; ++i)
			{
				wordWidth += glyphs_[pos + i].x_advance;
			}
			if (w + wordWidth > width) {
				if (isSpaceWord)
					return pos + wordCount;
				else
					return pos;
			}
			pos += wordCount;
			w += wordWidth;
		}
		
		return pos;
	}


	size_t UICairoTextRun::height() const
	{
		return fontExtents_.height;
	}

	size_t UICairoTextRun::ascent() const
	{
		return fontExtents_.ascent;
	}

	size_t UICairoTextRun::descent() const
	{
		return fontExtents_.descent;
	}

	size_t UICairoTextRun::calcRunWidth() const
	{
		size_t w = 0;
		for (int i = 0; i < glyphsCount_; ++i)
		{
			w += glyphs_[i].x_advance;
		}
		return w;
	}

	Size UICairoTextRun::calcRunSize() const
	{
		return Size(calcRunWidth(), height());
	}

	void UICairoTextRun::Render(cairo_t* cr)
	{
		cairo_glyph_t glyph;
		
// 		double x_bearing = glyphs_[0].x_bearing;
// 		double y_bearing = 0;
// 		for (int i = 0; i < glyphsCount_; ++i)
// 		{
// 			if (glyphs_[i].y_bearing < y_bearing)
// 				y_bearing = glyphs_[i].y_bearing;
// 		}

		double x_advance = -glyphs_[0].x_bearing;
		double y_advance = fontExtents_.ascent;
		for (int i = 0; i < glyphsCount_; ++i)
		{
			glyph.index = glyphs_[i].index;
			glyph.x = x_advance;
			glyph.y = y_advance;
			x_advance += glyphs_[i].x_advance;
			y_advance += glyphs_[i].y_advance;
			cairo_show_glyphs(cr, &glyph, 1);
		}
	}



	Rect UICairoTextPagraph::GetBoundsRect() const
	{
		size_t width = 0;
		size_t height = 0;
		for (UICairoTextRun* run = firstRun_; run; run = run->nextTextRun_)
		{
			Size sz(run->calcRunSize());
			if (sz.width() > width)
				width = sz.width();
			height += sz.height();
		}
		return Rect(0, 0, width, height);
	}

	void UICairoTextPagraph::Render(cairo_t* cr)
	{
		for (UICairoTextRun* run = firstRun_; run; run = run->nextTextRun_)
		{
			run->Render(cr);
			cairo_translate(cr, 0, run->height());
		}
	}

}