#pragma once
#include "uikit/text/glyph_fragment.h"

namespace ui
{
	class UIGlyphLine
	{
	public:
		UIGlyphLine();
		~UIGlyphLine();

		bool isEmpty() const { return fisrtGlyphFragment_ == NULL; }
// 		void setGlyphsCount(int s);
// 		size_t glyphsCount() const;
// 
// 		size_t calcTextWidth() const;
// 		size_t calcTextHeight() const;
// 		Size calcTextSize() const;
		int lineHeight() const;
		int lineWidth() const;
		Size lineSize() const;

		UIGlyphLine* nextGlyphLine() const;
		UIGlyphLine* prevGlyphLine() const;

		void clearGlyphFragment();
		void addGlyphFragment(UIGlyphFragment* fragment);

// 		size_t layoutGlyphSizeForWidth(size_t width);
// 		size_t layoutWordSizeForWidth(size_t width);
 		void Render(UIRenderContext* context);
		
	private:
		friend class UITextPagraph;
// 		const UIGlyph *glyphs_;
// 		const wchar_t* buffer_;
// 		size_t glyphsCount_;
// 		scoped_refptr<UIRenderFont> font_;

		UIGlyphLine* nextGlyphLine_{ NULL };
		UIGlyphLine* prevGlyphLine_{ NULL };

		UIGlyphFragment* fisrtGlyphFragment_{ NULL };
		UIGlyphFragment* lastGlyphFragment_{ NULL };
	};
}