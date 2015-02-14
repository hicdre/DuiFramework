#pragma once
#include "uikit/render/ui_render_font.h"
#include "uikit/render/cairo/cairo_forward.h"
#include "uikit/render/cairo/cairo_render_engine.h"

namespace ui
{
	class UICairoRenderFont : public UIRenderFont
	{
	public:
		UICairoRenderFont(UIFont* font, UICairoRenderEngine* engine);
		~UICairoRenderFont();

		virtual void GetGlyphs(const wchar_t* text, size_t len, 
			UIGlyph** glyphs, size_t& glyphsCount) override;

		virtual double GetHeight() override;
		virtual double GetAscent() override;
		virtual double GetDescent() override;

		cairo_scaled_font_t* cairoScaledFont();
		cairo_font_extents_t* cairoFontExtent();
	private:
		cairo_scaled_font_t* cairoScaledFont_{ NULL };
		cairo_font_extents_t* cairoFontExtent_{ NULL };
		scoped_refptr<UIFont> font_;
		UICairoRenderEngine* engine_;
	};
}