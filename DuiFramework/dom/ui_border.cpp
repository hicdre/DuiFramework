#include "stdafx.h"
#include "ui_border.h"
#include "dom/ui_include.h"
#include "render/render_include.h"

namespace ui
{
	void UIBorder::Init(UIStyles* styles)
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

	const UIBorder::Item& UIBorder::left() const
	{
		return left_;
	}

	const UIBorder::Item& UIBorder::top() const
	{
		return top_;
	}

	const UIBorder::Item& UIBorder::right() const
	{
		return right_;
	}

	const UIBorder::Item& UIBorder::bottom() const
	{
		return bottom_;
	}

	void UIBorder::InitItem(Item* item, StyleProperty width, StyleProperty color)
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

	void UIBorder::InitRadius(uint32& radius, StyleProperty p)
	{
		StyleValue* pixelValue = styles_->FindProperty(p);
		if (pixelValue && pixelValue->IsPixelValue()) {
			radius = pixelValue->GetPixel();
		}
		else {
			radius = 0;
		}
	}

	ui::Inseting UIBorder::GetPadding() const
	{
		return Inseting(left_.size, top_.size, right_.size, bottom_.size);
	}

	uint32 UIBorder::leftTopRadius() const
	{
		return left_top_radius_;
	}

	uint32 UIBorder::rightTopRadius() const
	{
		return right_top_radius_;
	}

	uint32 UIBorder::rightBottomRadius() const
	{
		return right_bottom_radius_;
	}

	uint32 UIBorder::leftBottomRadius() const
	{
		return left_bottom_radius_;
	}

	UIBorder::~UIBorder()
	{
	}

	UIBorder::UIBorder()
	{

	}

	UIBorderPainter::UIBorderPainter(const Rect& bounds, const UIBorder* border, RenderContext* context)
		: outBounds_(bounds)
		, inBounds_(bounds)
		, border_(border)
		, context_(context)
	{
		inBounds_.Inset(border_->GetPadding());
		Init();
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
		bool radiusSame = AllBorderRadiusSame();

		if (colorSame && radiusSame && border_->leftTopRadius() == 0) {
			// case 1 无圆角 同色
			scoped_refptr<RenderPath> path = context_->CreatePath();
			path->Rectangle(outBounds_);
			path->Rectangle(inBounds_, true);
			path->EndPath();
			context_->FillPath(path.get(), border_->left().color);
			return;
		}

		if (radiusSame && radiusSame == 0) {
			// case 2 圆角 同色
			scoped_refptr<RenderPath> path = context_->CreatePath();
			//path->RoundRectangle(outBounds_, );
			//path->RoundRectangle(inBounds_, true);
			path->EndPath();
			context_->FillPath(path.get(), border_->left().color);
			return;
		}

		
	}

	void UIBorderPainter::Init()
	{
	}

	bool UIBorderPainter::AllBorderWidthSame()
	{
		return (border_->left().size == border_->right().size)
			&& (border_->top().size == border_->bottom().size)
			&& (border_->left().size == border_->right().size);

	}

	bool UIBorderPainter::AllBorderColorSame()
	{
		return (border_->left().color == border_->right().color)
			&& (border_->top().color == border_->bottom().color)
			&& (border_->left().color == border_->right().color);
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

}