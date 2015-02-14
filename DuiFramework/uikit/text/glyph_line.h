#pragma once
#include "uikit/text/glyph_fragment.h"

namespace ui
{
	class UIGlyphLine
	{
	public:
		UIGlyphLine(const UIGlyph* glyphs,
			const wchar_t* buffer,
			size_t size, UIRenderFont* font);
		~UIGlyphLine();

		void setGlyphsCount(int s);
		size_t glyphsCount() const;

		size_t calcTextWidth() const;
		size_t calcTextHeight() const;
		Size calcTextSize() const;

		UIGlyphLine* nextTextLine() const;
		UIGlyphLine* prevTextLine() const;

		size_t layoutGlyphSizeForWidth(size_t width);
		size_t layoutWordSizeForWidth(size_t width);
		void Render(UIRenderContext* context, Color color);
	private:
		friend class UITextPagraph;
		const UIGlyph *glyphs_;
		const wchar_t* buffer_;
		size_t glyphsCount_;
		scoped_refptr<UIRenderFont> font_;

		UIGlyphLine* nextTextLine_{ NULL };
		UIGlyphLine* prevTextLine_{ NULL };
	};
}