#pragma once
#include "uikit/common/ui_font.h"
#include "uikit/common/ui_glyphs.h"
#include "uikit/render/ui_render_context.h"

namespace ui
{
	class UIGlyphFragment
	{
	public:
		UIGlyphFragment(UIGlyph* glyphs,
			const wchar_t* buffer,
			size_t size, UIFont* font);
		~UIGlyphFragment();

		void setGlyphsCount(int s);
		size_t glyphsCount() const;

		size_t calcTextWidth() const;
		Size calcTextSize() const;

		UIGlyphFragment* nextFragment() const;
		UIGlyphFragment* prevFragment() const;

		void Render(UIRenderContext* context, Color color);
	private:
		friend class UITextPagraph;
		UIGlyph *glyphs_;
		const wchar_t* string_;
		size_t glyphsCount_;
		scoped_refptr<UIFont> font_;

		UIGlyphFragment* nextFragment_{ NULL };
		UIGlyphFragment* prevFragment_{ NULL };
	};
}