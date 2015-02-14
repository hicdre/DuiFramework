#pragma once
#include "uikit/widget/ui_view.h"
#include "uikit/common/ui_font.h"
#include "uikit/render/ui_text_layout.h"
#include "uikit/common/ui_attribute_string.h"

namespace ui
{
	class UILabel : public UIView
	{
	public:
		UILabel();
		~UILabel();

		//@property std::wstring text;
		const std::wstring& text() const;
		void setText(const std::wstring& text);
		void setAttributeString(UIAttributeString* attributeString);

		//@property UIFont* font;
		UIFont* font() const { return font_; }
		void setFont(UIFont* font);

		//@property Color textColor;
		Color textColor() const { return textColor_; }
		void setTextColor(Color color);

		//@property TextAlignment textAlignment;
		TextAlignment textAlignment() const { return textAlignment_; }
		void setTextAlignment(TextAlignment v);

		//@property VerticalAlignment verticalAlignment;
		VerticalAlignment verticalAlignment() const { return verticalAligment_; }
		void setVerticalAlignment(VerticalAlignment v);

		//@property UILineBreakMode lineBreakMode;
		UILineBreakMode lineBreakMode() const { return lineBreakMode_; }
		void setLineBreakMode(UILineBreakMode v);

		virtual void OnPaint(UIRenderContext* painter) override;

		virtual Rect textRectForBounds(const Rect& rect);
		virtual void drawTextInRect(UIRenderContext* painter, const Rect& rect);
	private:
		UITextLayout* textLayout();
		void layoutText();
		std::wstring text_;

		scoped_ptr<UIAttributeString> attributeString_;
		scoped_refptr<UIFont> font_;
		Color textColor_{ Color_Black };
		TextAlignment textAlignment_{ TextAlignmentLeft };
		VerticalAlignment verticalAligment_{ VerticalAlignmentMiddle };
		scoped_refptr<UITextLayout> textLayout_;
		Rect textRectCached_;
		bool needsTextLayout_{ true };
		UILineBreakMode lineBreakMode_{ UILineBreakByWordWrapping };
	};
}