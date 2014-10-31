#pragma once
#include "base/basictypes.h"

namespace ui
{
	class RenderContext;
	/*	
		scope in view
		border-left-color
		border-left-width
		border-top-color
		border-top-width
		border-right-color
		border-right-width
		border-bottom-color
		border-bottom-width
	*/
	class Border
	{
	public:
		enum Direction{
			LEFT = 0,
			TOP,
			RIGHT,
			BOTTOM,
		};

		Border();
		~Border();

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

		Padding GetPadding();

		void DoPaint(RenderContext* painter, const Rect& dest);
	private:
		int border_size_[4];
		Color border_color_[4];
	};
}