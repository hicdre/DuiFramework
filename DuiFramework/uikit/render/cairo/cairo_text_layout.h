#pragma once

#include "uikit/render/ui_text_layout.h"
#include "uikit/render/cairo/cairo_forward.h"

namespace ui
{
	class UIRenderContext;
	class UICairoRenderEngine;
	class UICairoTextPagraph;
	typedef struct {
		uint32 index;
		double x_advance;
		double y_advance;
		double width;
		double height;
		double x_bearing;
		double y_bearing;
	} cairo_glyph_advance_t;

	class UICairoTextLayout : public UITextLayout
	{
	public:
		UICairoTextLayout(const UIFont* font, const Rect& rect, UICairoRenderEngine* engine);
		virtual ~UICairoTextLayout() override;

		//associate text
		virtual void SetText(const std::wstring& str) override;
		virtual void SetFont(const UIFont* font) override;
		virtual void SetBounds(const Rect& rect) override;
		virtual Rect Layout() override;
		virtual void Render(UIRenderContext* context, Color color) override;

	private:
		void RealLayout();
		void CleanLayoutResult();
		cairo_scaled_font_t* InitCairoFont();
		UICairoRenderEngine* engine() const { return engine_; }
		UICairoRenderEngine* engine_;
		scoped_refptr<UIFont> font_;
		Rect layoutBounds_;
		Rect textBounds_;
		std::wstring contents_;

		//result
		UICairoTextPagraph* textPagraph_;

		cairo_glyph_advance_t* layoudAdvance_;
		size_t glyphsCount_;

		bool needLayout_;
	};

	//表示一行字符
	class UICairoTextRun
	{
	public:
		UICairoTextRun(cairo_glyph_advance_t* glyphs, 
			const wchar_t* buffer,
			size_t size, const cairo_font_extents_t& fe)
			: glyphs_(glyphs)
			, glyphsCount_(size)
			, fontExtents_(fe)
			, buffer_(buffer)
		{}
		~UICairoTextRun() {}

		void setGlyphsCount(int s) { glyphsCount_ = s; }
		size_t glyphsCount() const { return glyphsCount_; }
		size_t height() const;
		size_t ascent() const;
		size_t descent() const;
		
		size_t calcRunWidth() const;
		Size calcRunSize() const;

		UICairoTextRun* nextTextRun() const { return nextTextRun_; }
		UICairoTextRun* prevTextRun() const { return prevTextRun_; }

		size_t layoutGlyphSizeForWidth(int width);
		size_t layoutWordSizeForWidth(int width);
		void Render(cairo_t* cr);
	private:
		friend class UICairoTextPagraph;
		cairo_glyph_advance_t *glyphs_;
		const wchar_t* buffer_;
		size_t glyphsCount_;
		cairo_font_extents_t fontExtents_;

		UICairoTextRun* nextTextRun_{ NULL };
		UICairoTextRun* prevTextRun_{ NULL };
	};


	class UICairoTextPagraph
	{
	public:
		UICairoTextPagraph() {}
		~UICairoTextPagraph() {
			destroy();
		}

		void addTextRun(UICairoTextRun* run) {
			if (lastRun_) {
				lastRun_->nextTextRun_ = run;
				run->prevTextRun_ = lastRun_;
				run->nextTextRun_ = NULL;
				lastRun_ = run;
			}
			else {
				assert(firstRun_ == NULL);
				firstRun_ = lastRun_ = run;
				run->prevTextRun_ = run->nextTextRun_ = NULL;
			}
			runCount_++;
		}

		size_t runCount() const { return runCount_; }
		bool isMutilLine() const { return runCount_ > 1; }

		Rect GetBoundsRect() const;

		void Render(cairo_t* cr);

		UICairoTextRun* firstRun() const { return firstRun_; }
		UICairoTextRun* lastRun() const { return lastRun_; }
	private:
		void destroy() {
			UICairoTextRun* run = firstRun_;
			while (run)
			{
				UICairoTextRun* currentRun = run;
				run = run->nextTextRun_;
				delete currentRun;
			}
			firstRun_ = lastRun_ = NULL;
			runCount_ = 0;
		}
		UICairoTextRun* firstRun_{ NULL };
		UICairoTextRun* lastRun_{ NULL };
		size_t runCount_{ 0 };
	};

}
