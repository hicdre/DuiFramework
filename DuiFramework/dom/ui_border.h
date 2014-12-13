#pragma once
#include "dom/ui_forward.h"
#include "style/style_result.h"
#include "render/render_engine.h"

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

		const Item& side(int index) const { return sides_[index]; }

		Inseting GetInseting() const;

		uint32 leftTopRadius() const;
		uint32 rightTopRadius() const;
		uint32 rightBottomRadius() const;
		uint32 leftBottomRadius() const;
	private:
		void InitItem(Item* item,
			StyleProperty width,
			StyleProperty color);
		void InitRadius(uint32& radius, StyleProperty p);
		union {
			struct {
				Item left_;
				Item top_;
				Item right_;
				Item bottom_;
			};
			Item sides_[4];
		};
		

		uint32 left_top_radius_;
		uint32 right_top_radius_;
		uint32 right_bottom_radius_;
		uint32 left_bottom_radius_;

		UIStyles* styles_;
	};

	class UIBorderPainter
	{
	public:
		UIBorderPainter(const Rect& bounds, const UIBorder* border, RenderContext* context);
		~UIBorderPainter();

		void Paint();
		scoped_refptr<RenderPath> ClipPath();
	private:
		void Init(const Rect& bounds);

		void PaintBorderSameColor(Color c);

		bool AllBorderWidthSame();
		bool AllBorderColorSame();
		bool AllBorderRadiusSame();

		bool AllBorderExistsColorSame();

		RoundRect outRoundRect_;
		RoundRect inRoundRect_;

		const UIBorder* border_;
		RenderContext* context_;
	};
}