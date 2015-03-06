#pragma once
#include "uikit/common/ui_font.h"
#include "uikit/common/ui_glyphs.h"
#include "uikit/common/ui_constants.h"
#include "uikit/render/ui_render_context.h"


namespace ui
{
	class UIGlyphLine;
	class TextFragment;
	//文字最小绘制单元
	class UIGlyphFragment
	{
	public:
		UIGlyphFragment(TextFragment* text,
			size_t offset,
			size_t count);
		~UIGlyphFragment();

		size_t glyphsCount() const;
		const UIGlyph* glyphs() const;

		int width() const;
		int height() const;

		UIGlyphFragment* nextFragment() const;
		UIGlyphFragment* prevFragment() const;

		UIGlyphFragment* splitAt(size_t pos);

		//调整为不大于width, 返回是否发生分割
		bool adjustForWidth(int width, UILineBreakMode mode);
		void Render(UIRenderContext* context);
	private:
		friend class TextPagraph;
		friend class UIGlyphLine;
		friend class UIGlyphPagraph;

		TextFragment* textFragment_;

		size_t glyphsOffset_;
		size_t glyphsCount_{ 0 };

		UIGlyphFragment* nextFragment_{ NULL };
		UIGlyphFragment* prevFragment_{ NULL };
	};
}