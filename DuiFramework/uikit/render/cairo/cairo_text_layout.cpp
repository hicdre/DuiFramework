#include "stdafx.h"
#include "cairo_text_layout.h"
#include "cairo_render_engine.h"
#include "cairo_render_context.h"

namespace ui
{


	UICairoTextLayout::UICairoTextLayout(const UIFont* font, 
		const Rect& rect,
		UICairoRenderEngine* engine)
		: layoutBounds_(rect)
		, engine_(engine)
		, needLayout_(true)
		, lineBreakMode_(UILineBreakByWordWrapping)
	{
		font_.reset(const_cast<UIFont*>(font));
	}


	UICairoTextLayout::~UICairoTextLayout()
	{
		CleanLayoutResult();
	}

	void UICairoTextLayout::SetText(const std::wstring& str)
	{
		if (str == contents_)
			return;
		contents_ = str;
		needLayout_ = true;
	}

	void UICairoTextLayout::SetFont(const UIFont* font)
	{
		if (font->IsEqual(font_))
			return;
		font_.reset(const_cast<UIFont*>(font));
		needLayout_ = true;
	}

	void UICairoTextLayout::SetLineBreakMode(UILineBreakMode mode)
	{
		if (lineBreakMode_ == mode)
			return;
		lineBreakMode_ = mode;
		needLayout_ = true;
	}


	void UICairoTextLayout::SetBounds(const Rect& rect)
	{
		if (layoutBounds_ == rect)
			return;
		layoutBounds_ = rect;
		needLayout_ = true;
	}

	Rect UICairoTextLayout::Layout()
	{
		if (!needLayout_)
			return textBounds_;

		RealLayout();
		needLayout_ = false;
		return textBounds_;
	}

	void UICairoTextLayout::Render(UIRenderContext* context, Color color)
	{
		UICairoRenderContext* cairoContext = dynamic_cast<UICairoRenderContext*>(context);
		if (!context)
			return;
		Layout();

		if (textStorage_ == NULL)// 空字符串
			return;

		
//		textStorage_->Render(context, color);
// 		cairo_t* cr = cairoContext->get_context();
// 		cairo_scaled_font_t* sf = InitCairoFont();
// 		cairoContext->InitColor(color);
// 		cairo_set_scaled_font(cr, sf);
// 
// 		cairo_save(cr);
// 		textPagraph_->Render(cr);
// 		cairo_restore(cr);
	}

	void UICairoTextLayout::CleanLayoutResult()
	{
		if (textStorage_)
		{
			delete textStorage_;
			textStorage_ = NULL;
		}

	}

	void UICairoTextLayout::RealLayout()
	{
// 		CleanLayoutResult();
// 
// 
// 		textStorage_ = new TextStorage;
// 		textStorage_->setText(contents_);
// 		textStorage_->Layout(layoutBounds_.size());
// 
// 		//utf8 => utf16
// 		Size layoutSize = textStorage_->GetLayoutSize();
// 
// 		if (layoutSize.height() > layoutBounds_.height()) {
// 			textBounds_.SetSize(layoutBounds_.width(), layoutSize.height());
// 		}
// 		else {
// 			textBounds_.SetSize(layoutSize.width(), layoutSize.height());
// 		}
	}
}