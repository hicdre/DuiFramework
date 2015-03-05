#pragma once
#include "uikit/common/ui_font.h"
#include "uikit/common/ui_glyphs.h"
#include "uikit/render/ui_render_context.h"

namespace ui
{
	//表示一段属性完全相同的文字
	class UITextFragment
	{
	public:
		UITextFragment(const wchar_t* text, size_t begin, size_t end);
		~UITextFragment();

		UITextFragment* prevTextFragment() const { return prev_; }
		UITextFragment* nextTextFragment() const { return next_; }

		//@property UIFont* font;
		UIFont* font() const { return font_; }
		void setFont(UIFont* font);

		//@property Color textColor;
		Color textColor() const { return textColor_; }
		void setTextColor(Color color);

		//@property Color backgroundColor;
		Color backgroundColor() const { return backgroundColor_; }
		void setBackgroundColor(Color color);

		size_t glyphsCount();
		bool propertyEquals(UITextFragment* fragment);

		//布局信息
		Size textSize();
		int textHeight();
		int textWidth();
		int textWidthWithRange(size_t begin, size_t end);

		
		//绘制
		void Render(UIRenderContext* context);
		void RenderWithRange(UIRenderContext* context, size_t begin, size_t end);
	private:
		friend class UITextPagraph;
		void clearGlyphs();
		void updateGlyphs();
		const wchar_t* str() const { return text_ + begin_; }
		size_t strLength() const { return end_ - begin_; }
		const wchar_t* text_;
		size_t begin_;
		size_t end_;

		scoped_refptr<UIFont> font_;
		Color textColor_{ Color_Black };
		Color backgroundColor_{ Color_Transparent };

		UITextFragment* next_{ NULL };
		UITextFragment* prev_{ NULL };

		//字形信息
		UIGlyph* glyphs_{ NULL };
		size_t glyphsCount_{ 0 };
		bool need_update_glyphs_{ true };
	};
}