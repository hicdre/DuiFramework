#pragma once
#include "uikit/text/text_fragment.h"
#include "uikit/text/glyph_line.h"
#include "uikit/common/ui_constants.h"

namespace ui
{
	class UITextPagraph
	{
	public:
		UITextPagraph(const wchar_t* text, size_t begin, size_t end);
		~UITextPagraph();

		const wchar_t* str() const { return firstTextFragment_ ? firstTextFragment_->str() : NULL; }
		size_t strLength() const;

		void SetWidth(size_t width);
		void SetPosition(int x, int y);
		int x() const { return bounds_.x(); }
		int y() const { return bounds_.y(); }

		UIGlyphLine* firstLine() const { return firstLine_; }
		UIGlyphLine* lastLine() const { return lastLine_; }
	
		size_t lineCount() const { return lineCount_; }
		bool isMutilLine() const { return lineCount_ > 1; }

		Rect GetBoundsRect() const;
		void Render(UIRenderContext* context);
	private:
		friend class UITextStorage;

		UIGlyphFragment* glyphFragmentForWidth(
			UITextFragment* textFragment,
			int &remain, size_t &pos);

		void addGlyphLine(UIGlyphLine* line);
		void addTextFragment(UITextFragment* fragment);
		void clearTextFragment();
		void Layout(size_t width, UILineBreakMode mode);
		void CalcBoundsRect();
		void clearGlyphLine();
		UITextPagraph* nextPagraph_{ NULL };
		UITextPagraph* prevPagraph_{ NULL };

// 		const wchar_t* text_;
// 		size_t begin_;
// 		size_t end_;

		UITextFragment* firstTextFragment_{ NULL };
		UITextFragment* lastTextFragment_{ NULL };

		UIGlyph* glyphs_;
		size_t glyphsCount_;

		UILineBreakMode lineBreakMode_{ UILineBreakByWordWrapping };
		UIGlyphLine* firstLine_{ NULL };
		UIGlyphLine* lastLine_{ NULL };
		size_t lineCount_{ 0 };

		Rect bounds_;
	};
}