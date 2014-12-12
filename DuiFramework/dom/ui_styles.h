#pragma once
#include "dom/ui_forward.h"
#include "style/style_result.h"
#include "dom/ui_border.h"

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
		const UIBorder* borders() const;

		CursorId cursor() const;

		Color backgroundColor() const;
		
		

		UIElement* element() const { return elem_; }
	private:
		friend class UIBorder;
		void InitStyles();
		void InitCursor();
		StyleValue* FindProperty(StyleProperty p) const;
		Rect GetParentContentBounds() const;
		int GetMarginValue(StyleProperty p) const;
		
		UIBorder borders_;
		CursorId cursor_;

		UIElement* elem_;
		StyleResultList results_;
	};

}