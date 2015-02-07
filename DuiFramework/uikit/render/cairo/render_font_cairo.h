#pragma once
#include "render/render_font.h"

namespace ui
{
	class RenderFontCairo : public RenderFont
	{
	public:
		RenderFontCairo(const UIFont& font);
		~RenderFontCairo();

		static scoped_refptr<RenderFont> Create(const UIFont& font);

		virtual void GetGlyphIndices(const RenderContext* context,
			const wchar_t* str, size_t len, uint32* glyphs) override;


	private:
		void Initialize();
		UIFont font_;
		//cairo_font_face_t* cairo_face_;
	};
}