#pragma once

#include "uikit/render/ui_text_layout.h"
#include "uikit/render/cairo/cairo_forward.h"

namespace ui
{
	class UIRenderContext;
	class UICairoRenderEngine;
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
		typedef struct {
			int num_wchars;
			int num_glyphs;
		} cairo_utf16_cluster;
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
		cairo_glyph_t *layoutGlyphs_;
		int layoutGlyphsNum_;
		cairo_utf16_cluster* textClusters_;
		int textClustersNum_;

		bool needLayout_;
	};
}
