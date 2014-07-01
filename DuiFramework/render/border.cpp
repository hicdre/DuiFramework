#include "stdafx.h"
#include "border.h"

#include "core/view.h"
#include "render/painter.h"

namespace ui
{

	Border::Border()
	{
		SetBorder(0, ColorSetRGB(0, 0, 0));
	}

	Border::~Border()
	{

	}

	void Border::SetBorder(Direction direction, int size, Color color)
	{
		border_size_[direction] = size;
		border_color_[direction] = color;
	}

	void Border::SetBorder(int size, Color color)
	{
		SetBorder(LEFT, size, color);
		SetBorder(TOP, size, color);
		SetBorder(RIGHT, size, color);
		SetBorder(BOTTOM, size, color);
	}

	int Border::size(Direction direction) const
	{
		return border_size_[direction];
	}

	Color Border::color(Direction direction) const
	{
		return border_color_[direction];
	}

	void Border::DoPaint(View* view, Painter* painter)
	{
		//ио->вС->об->ср
		painter->FillRect(Rect(0, 0, view->width(), top()), top_color());
		painter->FillRect(Rect(0, 0, left(), view->height()), left_color());
		painter->FillRect(Rect(0, view->height() - bottom(), view->width(),
			bottom()), bottom_color());
		painter->FillRect(Rect(view->width() - right(), 0, right(),
			view->height()), right_color());
	}

	int Border::left() const
	{
		return size(LEFT);
	}

	int Border::top() const
	{
		return size(TOP);
	}

	int Border::right() const
	{
		return size(RIGHT);
	}

	int Border::bottom() const
	{
		return size(BOTTOM);
	}

	Color Border::left_color() const
	{
		return color(LEFT);
	}

	Color Border::top_color() const
	{
		return color(TOP);
	}

	Color Border::right_color() const
	{
		return color(RIGHT);
	}

	Color Border::bottom_color() const
	{
		return color(BOTTOM);
	}

}