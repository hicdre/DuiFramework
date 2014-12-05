#pragma once
#include "dom/ui_forward.h"
#include "style/style_rule.h"

namespace ui
{

	struct StyleRuleCompare
	{
		bool operator() (const StyleRule* a, const StyleRule* b) const
		{
			return  a < b;
		}
	};

	class RenderStyles
	{
	public:
		RenderStyles(UIElement* elem);
		~RenderStyles();

		void AddStyleRule(StyleRule* rule);
		void Reset();

		int marginLeft() const;
		int marginTop() const;
		int marginRight() const;
		int marginBottom() const;

		int pixelWidth() const;
		bool autoWidth() const;
		int pixelHeight() const;
		bool autoHeight() const;

		Color backgroundColor() const;
		
	private:
		Rect GetParentContentBounds() const;
		int GetMarginValue(StyleProperty p) const;

		StyleValue* FindProperty(StyleProperty p) const;

		UIElement* elem_;
		std::set<StyleRule*, StyleRuleCompare> rules_;
	};
}