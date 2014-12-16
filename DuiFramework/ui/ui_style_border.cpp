#include "stdafx.h"
#include "ui_style_border.h"
#include "ui/ui_include.h"
#include "render/render_include.h"

namespace ui
{
	void UIStyleBorder::Init(UIStyles* styles)
	{
		styles_ = styles;
		InitItem(&left_, Style_BorderLeftWidth, Style_BorderLeftColor);
		InitItem(&top_, Style_BorderTopWidth, Style_BorderTopColor);
		InitItem(&right_, Style_BorderRightWidth, Style_BorderRightColor);
		InitItem(&bottom_, Style_BorderBottomWidth, Style_BorderBottomColor);

		InitRadius(left_top_radius_, Style_BorderLeftTopRadius);
		InitRadius(right_top_radius_, Style_BorderRightTopRadius);
		InitRadius(right_bottom_radius_, Style_BorderRightBottomRadius);
		InitRadius(left_bottom_radius_, Style_BorderLeftBottomRadius);

	}

	const UIStyleBorder::Item& UIStyleBorder::left() const
	{
		return left_;
	}

	const UIStyleBorder::Item& UIStyleBorder::top() const
	{
		return top_;
	}

	const UIStyleBorder::Item& UIStyleBorder::right() const
	{
		return right_;
	}

	const UIStyleBorder::Item& UIStyleBorder::bottom() const
	{
		return bottom_;
	}

	void UIStyleBorder::InitItem(Item* item, StyleProperty width, StyleProperty color)
	{
		StyleValue* widthValue = styles_->FindProperty(width);
		if (widthValue && widthValue->IsPixelValue()) {
			item->size = widthValue->GetPixel();
		}
		else {
			item->size = 0;
		}

		StyleValue* colorValue = styles_->FindProperty(color);
		if (colorValue && colorValue->IsColorValue()) {
			item->color = colorValue->GetColorValue();
		}
		else {
			item->color = Color_Transparent;
		}
	}

	void UIStyleBorder::InitRadius(uint32& radius, StyleProperty p)
	{
		StyleValue* pixelValue = styles_->FindProperty(p);
		if (pixelValue && pixelValue->IsPixelValue()) {
			radius = pixelValue->GetPixel();
		}
		else {
			radius = 0;
		}
	}

	ui::Inseting UIStyleBorder::GetInseting() const
	{
		return Inseting(left_.size, top_.size, right_.size, bottom_.size);
	}

	uint32 UIStyleBorder::leftTopRadius() const
	{
		return left_top_radius_;
	}

	uint32 UIStyleBorder::rightTopRadius() const
	{
		return right_top_radius_;
	}

	uint32 UIStyleBorder::rightBottomRadius() const
	{
		return right_bottom_radius_;
	}

	uint32 UIStyleBorder::leftBottomRadius() const
	{
		return left_bottom_radius_;
	}

	UIStyleBorder::~UIStyleBorder()
	{
	}

	UIStyleBorder::UIStyleBorder()
	{

	}

	UIBorderPainter::UIBorderPainter(const Rect& bounds, const UIStyleBorder* border, RenderContext* context)
		: border_(border)
		, context_(context)
	{
		Init(bounds);
	}

	UIBorderPainter::~UIBorderPainter()
	{

	}

	void UIBorderPainter::Paint()
	{
		bool widthSame = AllBorderWidthSame();
		if (widthSame && border_->left().size == 0) // empty
			return;

		bool colorSame = AllBorderColorSame();

		if (colorSame) {
			// case 1 无圆角 同色
			scoped_refptr<RenderPath> path = context_->CreatePath();
			path->RoundRectangle(outRoundRect_);
			path->RoundRectangle(inRoundRect_, true);
			path->EndPath();
			context_->FillPath(path.get(), border_->left().color);
			return;
		}

		
	}

	void UIBorderPainter::Init(const Rect& bounds)
	{
		RoundRect roundRect(bounds
			, border_->leftTopRadius()
			, border_->rightTopRadius()
			, border_->rightBottomRadius()
			, border_->leftBottomRadius());

		outRoundRect_ = roundRect;

		roundRect.Inset(border_->GetInseting());
		inRoundRect_ = roundRect;
	}

	bool UIBorderPainter::AllBorderWidthSame()
	{
		return (border_->left().size == border_->right().size)
			&& (border_->top().size == border_->bottom().size)
			&& (border_->left().size == border_->right().size);

	}

	bool UIBorderPainter::AllBorderColorSame()
	{
		bool first_inited = false;
		Color color = Color_Transparent;
		for (int i = 0; i < 4; i++)
		{
			const UIStyleBorder::Item& item = border_->side(i);
			if (item.size == 0)
				continue;

			if (!first_inited) {
				color = item.color;
				first_inited = true;
				continue;
			}

			if (color != item.color)
				return false;
		}

		return true;
	}

	bool UIBorderPainter::AllBorderRadiusSame()
	{
		return (border_->leftTopRadius() == border_->rightTopRadius())
			&& (border_->rightBottomRadius() == border_->leftBottomRadius())
			&& (border_->leftTopRadius() == border_->leftBottomRadius());
	}

	void UIBorderPainter::PaintBorderSameColor(Color c)
	{
		if (AllBorderRadiusSame() && border_->leftTopRadius() == 0) {
			//no.1 无圆角
			
		}


	}

	scoped_refptr<RenderPath> UIBorderPainter::ClipPath()
	{
		scoped_refptr<RenderPath> path = context_->CreatePath();
		path->RoundRectangle(inRoundRect_);
		path->EndPath();
		return path;
	}

}