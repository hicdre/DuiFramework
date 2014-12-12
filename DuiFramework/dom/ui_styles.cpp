#include "stdafx.h"
#include "ui_styles.h"
#include "dom/ui_include.h"
#include "render/render_include.h"

namespace ui
{


	UIStyles::UIStyles(UIElement* elem)
		: elem_(elem)
		, cursor_(Cursor_Inherit)
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
		if (v->IsPixelValue()) {
			return v->GetPixel();
		}

		if (v->IsPercentValue() && elem_->HasParent())	{
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
			
		if (v->IsPixelValue()) {
			return v->GetPixel();
		}

		Rect rc(GetParentContentBounds());
		if (v->IsPercentValue()) {
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
		if (v->IsPixelValue()) {
			return v->GetPixel();
		}

		Rect rc(GetParentContentBounds());
		if (v->IsPercentValue()) {
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

		InitStyles();

		//onstylechange
		elem_->SchedulePaint();

	}


	void UIStyles::InitStyles()
	{
		borders_.Init(this);

		InitCursor();
	}


	const UIBorder* UIStyles::borders() const
	{
		return &borders_;
	}

	ui::CursorId UIStyles::cursor() const
	{
		return cursor_;
	}

	void UIStyles::InitCursor()
	{
		StyleValue* v = FindProperty(Style_Cursor);
		if (v && v->IsCursorValue()) {
			cursor_ = v->GetCursorValue();
		}
		else {
			cursor_ = Cursor_Inherit;
		}
	}


	

}