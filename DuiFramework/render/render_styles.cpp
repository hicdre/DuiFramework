#include "stdafx.h"
#include "render_styles.h"
#include "dom/ui_include.h"
#include "render/render_include.h"

namespace ui
{


	RenderStyles::RenderStyles(UIElement* elem)
		: elem_(elem)
	{
		elem->GetDocument()->SelectStyles(elem, this);
	}

	RenderStyles::~RenderStyles()
	{
		Reset();
	}

	int RenderStyles::marginLeft() const
	{
		return GetMarginValue(Style_MarginLeft);
	}

	int RenderStyles::marginTop() const
	{
		return GetMarginValue(Style_MarginTop);
	}

	int RenderStyles::marginRight() const
	{
		return GetMarginValue(Style_MarginRight);
	}

	int RenderStyles::marginBottom() const
	{
		return GetMarginValue(Style_MarginBottom);
	}

	int RenderStyles::GetMarginValue(StyleProperty p) const
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

	void RenderStyles::AddStyleRule(StyleRule* rule)
	{
		for (StyleRule* r : rules_)
		{
			if (r == rule) {
				return;
			}
		}
		rules_.push_back(rule);
		rule->AddRef();
	}


	void RenderStyles::Reset()
	{
		for (StyleRule* rule : rules_)
		{
			rule->Release();
		}
		rules_.clear();
	}


	StyleValue* RenderStyles::FindProperty(StyleProperty p) const
	{
		for (StyleRule* rule : rules_)
		{
			StyleValue* v = rule->FindValue(p);
			if (v)
				return v;
		}
		return NULL;
	}

	int RenderStyles::pixelWidth() const
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

	int RenderStyles::pixelHeight() const
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

	bool RenderStyles::autoWidth() const
	{
		StyleValue* v = FindProperty(Style_Width);
		if (!v)
			return true;

		return v->IsAutoValue();
	}

	bool RenderStyles::autoHeight() const
	{
		StyleValue* v = FindProperty(Style_Height);
		if (!v)
			return true;

		return v->IsAutoValue();
	}

	Rect RenderStyles::GetParentContentBounds() const
	{
		if (!elem_->HasParent())
			return Rect();
		return elem_->parent()->GetContentBounds();
	}

	Color RenderStyles::backgroundColor() const
	{
		StyleValue* color = FindProperty(Style_BackgroundColor);
		if (color && color->IsColorValue())
		{
			return color->GetColorValue();
		}

		return Color_Transparent;
	}

	

}