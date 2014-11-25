#include "stdafx.h"
#include "render_styles.h"
#include "dom/dom_include.h"

namespace ui
{


	RenderStyles::RenderStyles(UIElement* elem)
		: elem_(elem)
	{
		elem->GetDocument()->SelectStyles(elem, this);
	}

	RenderStyles::~RenderStyles()
	{
		for (StyleRule* rule : rules_)
		{
			rule->Release();
		}
		rules_.clear();
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
			Rect rc(elem_->parent()->GetContentBounds());
			if (p == Style_MarginLeft || p == Style_MarginRight)
				return rc.width() * v->GetPercentValue();
			else if (p == Style_MarginTop || p == Style_MarginBottom)
				return rc.height() * v->GetPercentValue();
		}
		return 0;
	}

	void RenderStyles::AddStyleRule(StyleRule* rule)
	{
		rules_.push_back(rule);
		rule->AddRef();
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

}