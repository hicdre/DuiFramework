#pragma once
#include "render/color.h"
#include "render/rect.h"

namespace ui
{
	class View;
	class Painter;
	class Border
	{
	public:
		virtual ~Border() {}
		virtual void DoPaint(View* view, Painter* painter) = 0;
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

		virtual void DoPaint(View* view, Painter* painter) override;
	private:
		int border_size_[4];
		Color border_color_[4];
	};
}