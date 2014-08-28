#pragma once
#include "render/color.h"
#include "render/rect.h"
#include "core/drawable.h"

namespace ui
{
	class Painter;
	class Border : public Drawable
	{
	public:
		virtual ~Border() {}
		virtual Padding GetPadding() = 0;
	};

	class NormalBorder : public Border
	{
	public:
		enum Direction{
			LEFT = 0,
			TOP,
			RIGHT,
			BOTTOM,
		};

		NormalBorder();
		~NormalBorder();

		void SetBorder(Direction direction, int size, Color color);
		void SetBorder(int size, Color color);

		int size(Direction direction) const;
		Color color(Direction direction) const;

		int left() const;
		int top() const;
		int right() const;
		int bottom() const;

		Color left_color() const;
		Color top_color() const;
		Color right_color() const;
		Color bottom_color() const;

		virtual void DoPaint(Painter* painter, const Rect& dest) override;
		virtual Padding GetPadding() override;
	private:
		int border_size_[4];
		Color border_color_[4];
	};
}