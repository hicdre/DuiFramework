#pragma once
#include "uikit/text/text_fragment.h"
#include "uikit/text/glyph_line.h"
#include "uikit/common/ui_constants.h"

namespace ui
{
	class UIGlyphPagraph;
	class TextPagraph
	{
	public:
		TextPagraph(const wchar_t* text, size_t begin, size_t end);
		~TextPagraph();

		const wchar_t* str() const { return firstTextFragment_ ? firstTextFragment_->str() : NULL; }
		size_t strLength() const;


// 		Rect GetBoundsRect() const;
// 		void Render(UIRenderContext* context);

		void addTextFragment(TextFragment* fragment);
		void clearTextFragment();

		UIGlyphPagraph* buildGlyphPagraph();
	private:
		friend class TextStorage;
		friend class TextDocument;

// 		void CalcBoundsRect();
// 		void clearGlyphLine();
		TextPagraph* nextPagraph_{ NULL };
		TextPagraph* prevPagraph_{ NULL };

		TextFragment* firstTextFragment_{ NULL };
		TextFragment* lastTextFragment_{ NULL };

// 		UIGlyph* glyphs_;
// 		size_t glyphsCount_;

//		UILineBreakMode lineBreakMode_{ UILineBreakByWordWrapping };

	};
}