#pragma once
#include "dom/ui_forward.h"
#include "style/style_result.h"

namespace ui
{
	class UIStyles;
	class UIBorder
	{
	public:
		struct Item{
			int size;
			Color color;
		};
		UIBorder();
		~UIBorder();

		void Init(UIStyles* styles);

		const Item& left() const;
		const Item& top() const;
		const Item& right() const;
		const Item& bottom() const;

		Inseting GetPadding() const;

		uint32 leftTopRadius() const;
		uint32 rightTopRadius() const;
		uint32 rightBottomRadius() const;
		uint32 leftBottomRadius() const;

		HRGN CreateClipRgn() const;
	private:
		void InitItem(Item* item, 
			StyleProperty width,
			StyleProperty color);
		void InitRadius(uint32& radius, StyleProperty p);
		Item left_;
		Item top_;
		Item right_;
		Item bottom_;

		uint32 left_top_radius_;
		uint32 right_top_radius_;
		uint32 right_bottom_radius_;
		uint32 left_bottom_radius_;

		UIStyles* styles_;
	};


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