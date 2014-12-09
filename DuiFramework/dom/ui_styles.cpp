#include "stdafx.h"
#include "ui_styles.h"
#include "dom/ui_include.h"
#include "render/render_include.h"

namespace ui
{


	UIStyles::UIStyles(UIElement* elem)
		: elem_(elem)
	{
		//elem->GetDocument()->SelectStyles(elem, this);
	}

	UIStyles::~UIStyles()
	{
	}

	int UIStyles::marginLeft() const
	{
		return GetMarginValue(Style_MarginLeft);
	}

	int UIStyles::marginTop() const
	{
		return GetMarginValue(Style_MarginTop);
	}

	int UIStyles::marginRight() const
	{
		return GetMarginValue(Style_MarginRight);
	}

	int UIStyles::marginBottom() const
	{
		return GetMarginValue(Style_MarginBottom);
	}

	int UIStyles::GetMarginValue(StyleProperty p) const
	{
		StyleValue* v = FindProperty(p);
		if (!v)
			return 0;
		StyleValueType type = v->GetType();
		if (type == StyleValue_Pixel) {
			return v->GetPixel();
		}

		if (type == StyleValue_Percent && elem_->HasParent())	{
			Rect rc(GetParentContentBounds());
			if (p == Style_MarginLeft || p == Style_MarginRight)
				return rc.width() * v->GetPercentValue();
			else if (p == Style_MarginTop || p == Style_MarginBottom)
				return rc.height() * v->GetPercentValue();
		}
		return 0;
	}

	StyleValue* UIStyles::FindProperty(StyleProperty p) const
	{
		return results_.GetValue(p);
	}

	int UIStyles::pixelWidth() const
	{
		StyleValue* v = FindProperty(Style_Width);
		if (!v)
			return 0;
			
		StyleValueType type = v->GetType();
		if (type == StyleValue_Pixel) {
			return v->GetPixel();
		}

		Rect rc(GetParentContentBounds());
		if (type == StyleValue_Percent) {
			return rc.width() * v->GetPercentValue();
		}

		return 0;
	}

	int UIStyles::pixelHeight() const
	{
		StyleValue* v = FindProperty(Style_Height);
		if (!v)
			return 0;

		StyleValueType	type = v->GetType();
		if (type == StyleValue_Pixel) {
			return v->GetPixel();
		}

		Rect rc(GetParentContentBounds());
		if (type == StyleValue_Percent) {
			return rc.height() * v->GetPercentValue();
		}

		return 0;
	}

	bool UIStyles::autoWidth() const
	{
		StyleValue* v = FindProperty(Style_Width);
		if (!v)
			return true;

		return v->IsAutoValue();
	}

	bool UIStyles::autoHeight() const
	{
		StyleValue* v = FindProperty(Style_Height);
		if (!v)
			return true;

		return v->IsAutoValue();
	}

	Rect UIStyles::GetParentContentBounds() const
	{
		if (!elem_->HasParent())
			return Rect();
		return elem_->parent()->GetContentBounds();
	}

	Color UIStyles::backgroundColor() const
	{
		StyleValue* color = FindProperty(Style_BackgroundColor);
		if (color && color->IsColorValue())
		{
			return color->GetColorValue();
		}

		return Color_Transparent;
	}


	void UIStyles::UpdateStyles(StyleResultList* results)
	{
		std::set<StyleProperty> changed;
		for (int i = Style_BEGIN; i != Style_END; ++i)
		{
			StyleProperty p = (StyleProperty)i;
			StyleValue* old = results_.GetValue(p);
			StyleValue* current = results->GetValue(p);
			if (old != current)
				changed.insert((StyleProperty)p);
		}

		results_.Swap(results);

		borders_.Init(this);

		//onstylechange
		elem_->SchedulePaint();

	}

	const UIBorder* UIStyles::borders() const
	{
		return &borders_;
	}

	


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
		if (widthValue && widthValue->GetType() == StyleValue_Pixel) {
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
		if (pixelValue && pixelValue->GetType() == StyleValue_Pixel) {
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

	HRGN UIBorder::CreateClipRgn() const
	{
		Rect rect(styles_->element()->GetLocalBounds());
		HRGN rgn = CreateRectRgn(rect.x(), rect.y(), rect.right(), rect.bottom());
		if (left_top_radius_) {
			Rect cornerRect(rect.x(), rect.y(), left_top_radius_, left_top_radius_);
			Rect arcRect(cornerRect.x(), cornerRect.y(), cornerRect.width() * 2, cornerRect.height() * 2);
			HRGN cornerRgn = CreateRectRgn(cornerRect.x(), cornerRect.y(), cornerRect.right(), cornerRect.bottom());
			HRGN arcRgn = CreateEllipticRgn(arcRect.x(), arcRect.y(), arcRect.right(), arcRect.bottom());
			CombineRgn(cornerRgn, cornerRgn, arcRgn, RGN_DIFF);
			CombineRgn(rgn, rgn, cornerRgn, RGN_DIFF);
			DeleteObject(cornerRgn);
			DeleteObject(arcRgn);
		}

		if (right_top_radius_) {
			Rect cornerRect(rect.right() - right_top_radius_, rect.y(), right_top_radius_, right_top_radius_);
			Rect arcRect(cornerRect.x() - cornerRect.width(), cornerRect.y(), cornerRect.width() * 2, cornerRect.height() * 2);
			HRGN cornerRgn = CreateRectRgn(cornerRect.x(), cornerRect.y(), cornerRect.right(), cornerRect.bottom());
			HRGN arcRgn = CreateEllipticRgn(arcRect.x(), arcRect.y(), arcRect.right(), arcRect.bottom());
			CombineRgn(cornerRgn, cornerRgn, arcRgn, RGN_DIFF);
			CombineRgn(rgn, rgn, cornerRgn, RGN_DIFF);
			DeleteObject(cornerRgn);
			DeleteObject(arcRgn);
		}

		if (right_bottom_radius_) {
			Rect cornerRect(rect.right() - right_bottom_radius_, rect.y() - right_bottom_radius_, right_bottom_radius_, right_bottom_radius_);
			Rect arcRect(cornerRect.x() - cornerRect.width(), cornerRect.y() - cornerRect.height(), cornerRect.width() * 2, cornerRect.height() * 2);
			HRGN cornerRgn = CreateRectRgn(cornerRect.x(), cornerRect.y(), cornerRect.right(), cornerRect.bottom());
			HRGN arcRgn = CreateEllipticRgn(arcRect.x(), arcRect.y(), arcRect.right(), arcRect.bottom());
			CombineRgn(cornerRgn, cornerRgn, arcRgn, RGN_DIFF);
			CombineRgn(rgn, rgn, cornerRgn, RGN_DIFF);
			DeleteObject(cornerRgn);
			DeleteObject(arcRgn);
		}

		if (left_bottom_radius_) {
			Rect cornerRect(rect.x(), rect.y() - left_bottom_radius_, left_bottom_radius_, left_bottom_radius_);
			Rect arcRect(cornerRect.x(), cornerRect.y() - cornerRect.height(), cornerRect.width() * 2, cornerRect.height() * 2);
			HRGN cornerRgn = CreateRectRgn(cornerRect.x(), cornerRect.y(), cornerRect.right(), cornerRect.bottom());
			HRGN arcRgn = CreateEllipticRgn(arcRect.x(), arcRect.y(), arcRect.right(), arcRect.bottom());
			CombineRgn(cornerRgn, cornerRgn, arcRgn, RGN_DIFF);
			CombineRgn(rgn, rgn, cornerRgn, RGN_DIFF);
			DeleteObject(cornerRgn);
			DeleteObject(arcRgn);
		}
		return rgn;
	}

}