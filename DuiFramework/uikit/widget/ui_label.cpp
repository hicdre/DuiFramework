#include "stdafx.h"
#include "ui_label.h"
#include "uikit/render/ui_render_engine.h"

namespace ui
{
	UILabel::UILabel()
		: font_(UIFont::CreateDefault())
	{

	}

	UILabel::~UILabel()
	{

	}

	void UILabel::setText(const std::wstring& text)
	{
		if (text_ == text)
			return;
		text_ = text;
		needsTextLayout_ = true;
		SchedulePaint();
	}

	void UILabel::setFont(UIFont* font)
	{
		if (font->IsEqual(font_))
			return;

		font_.reset(font);
		needsTextLayout_ = true;
		SchedulePaint();
	}

	void UILabel::setTextColor(Color color)
	{
		if (textColor_ == color)
			return;

		textColor_ = color;
		SchedulePaint();
	}

	void UILabel::OnPaint(UIRenderContext* painter)
	{
		UIView::OnPaint(painter);

		layoutText();
		drawTextInRect(painter, textRectCached_);
	}

	void UILabel::setVerticalAlignment(TextAlignment v)
	{
		if (textAlignment_ == v)
			return;
		textAlignment_ = v;
	}

	void UILabel::setVerticalAlignment(VerticalAlignment v)
	{
		if (verticalAligment_ == v)
			return;
		verticalAligment_ = v;
	}

	Rect UILabel::textRectForBounds(const Rect& boundsRect)
	{
		UITextLayout* layout = textLayout();
		layout->SetBounds(boundsRect);
		Rect textRect = layout->Layout();
		if (!(textAlignment_ == TextAlignmentLeft || textRect.width() == boundsRect.width()))
		{
			int delta = boundsRect.width() - textRect.width();
			if (textAlignment_ == TextAlignmentCenter) {
				textRect.Offset(delta / 2, 0);
			}
			else { // TextAlignmentRight
				textRect.Offset(delta, 0);
			}
		}

		if (!(verticalAligment_ == VerticalAlignmentTop || textRect.height() == boundsRect.height()))
		{
			int delta = boundsRect.height() - textRect.height();
			if (verticalAligment_ == VerticalAlignmentMiddle) {
				textRect.Offset(0, delta / 2);
			}
			else { // VerticalAlignmentBottom
				textRect.Offset(0, delta);
			}
		}
		return textRect;
	}

	void UILabel::drawTextInRect(UIRenderContext* painter, const Rect& rect)
	{
		//painter->DrawText(text_, font_, textColor_);
		UIScopedPainter helper(painter, Matrix(1.0, 0, 0, 1.0, rect.x(), rect.y()));
		textLayout()->Render(painter, textColor());
	}

	UITextLayout* UILabel::textLayout()
	{
		if (!textLayout_)
		{
			textLayout_ = UIRenderEngine::current()->CreateRenderTextLayout(font_, GetLocalBounds());
		}
		return textLayout_;
	}

	void UILabel::layoutText()
	{
		if (!needsTextLayout_)
			return;

		needsTextLayout_ = false;
		textLayout()->SetText(text_);
		Rect rect = textRectForBounds(GetLocalBounds());
		if (rect == textRectCached_)
			return;
		textRectCached_ = rect;
		SchedulePaint();
	}

}