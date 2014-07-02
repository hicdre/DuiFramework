#include "stdafx.h"
#include "border.h"

#include "core/view.h"
#include "render/painter.h"

namespace ui
{

	NormalBorder::NormalBorder()
	{
		SetBorder(0, ColorSetRGB(0, 0, 0));
	}

	NormalBorder::~NormalBorder()
	{

	}

	void NormalBorder::SetBorder(Direction direction, int size, Color color)
	{
		border_size_[direction] = size;
		border_color_[direction] = color;
	}

	void NormalBorder::SetBorder(int size, Color color)
	{
		SetBorder(LEFT, size, color);
		SetBorder(TOP, size, color);
		SetBorder(RIGHT, size, color);
		SetBorder(BOTTOM, size, color);
	}

	int NormalBorder::size(Direction direction) const
	{
		return border_size_[direction];
	}

	Color NormalBorder::color(Direction direction) const
	{
		return border_color_[direction];
	}

	void NormalBorder::DoPaint(View* view, Painter* painter)
	{
		//ио->вС->об->ср
		painter->FillRect(Rect(0, 0, view->width(), top()), top_color());
		painter->FillRect(Rect(0, 0, left(), view->height()), left_color());
		painter->FillRect(Rect(0, view->height() - bottom(), view->width(),
			bottom()), bottom_color());
		painter->FillRect(Rect(view->width() - right(), 0, right(),
			view->height()), right_color());
	}

	int NormalBorder::left() const
	{
		return size(LEFT);
	}

	int NormalBorder::top() const
	{
		return size(TOP);
	}

	int NormalBorder::right() const
	{
		return size(RIGHT);
	}

	int NormalBorder::bottom() const
	{
		return size(BOTTOM);
	}

	Color NormalBorder::left_color() const
	{
		return color(LEFT);
	}

	Color NormalBorder::top_color() const
	{
		return color(TOP);
	}

	Color NormalBorder::right_color() const
	{
		return color(RIGHT);
	}

	Color NormalBorder::bottom_color() const
	{
		return color(BOTTOM);
	}

}