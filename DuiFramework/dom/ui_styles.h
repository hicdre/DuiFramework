#pragma once
#include "dom/ui_forward.h"
#include "style/style_result.h"
#include "dom/ui_style_border.h"
#include "dom/ui_style_background.h"

namespace ui
{
	class UIStyles
	{
	public:
		UIStyles(UIElement* elem);
		~UIStyles();

		void UpdateStyles(StyleResultList* results);

		int marginLeft() const;
		int marginTop() const;
		int marginRight() const;
		int marginBottom() const;

		int pixelWidth() const;
		bool autoWidth() const;
		int pixelHeight() const;
		bool autoHeight() const;

		//border
		const UIStyleBorder* borders() const;
		const UIStyleBackground* background() const;

		CursorId cursor() const;

		UIElement* element() const { return elem_; }
	private:
		friend class UIStyleBorder;
		friend class UIStyleBackground;
		void InitStyles();
		void InitCursor();
		StyleValue* FindProperty(StyleProperty p) const;
		Rect GetParentContentBounds() const;
		int GetMarginValue(StyleProperty p) const;
		
		UIStyleBorder borders_;
		UIStyleBackground background_;
		CursorId cursor_;

		UIElement* elem_;
		StyleResultList results_;
	};

}