#include "stdafx.h"
#include "view_border.h"

#include "render/render_context.h"

namespace ui
{

	Border::Border()
	{
		SetBorder(0, Color_Transparent);
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

	void Border::DoPaint(RenderContext* painter, const Rect& dest)
	{
		//ио->вС->об->ср
		painter->FillRect(Rect(0, 0, dest.width(), top()), top_color());
		painter->FillRect(Rect(0, 0, left(), dest.height()), left_color());
		painter->FillRect(Rect(0, dest.height() - bottom(), dest.width(),
			bottom()), bottom_color());
		painter->FillRect(Rect(dest.width() - right(), 0, right(),
			dest.height()), right_color());
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

	ui::Inseting Border::GetPadding()
	{
		return Inseting(left(), top(), right(), bottom());
	}

	void Border::SetBorderColor(Direction direction, Color color)
	{
		border_color_[direction] = color;
	}

	void Border::SetBorderWidth(Direction direction, int size)
	{
		border_size_[direction] = size;
		
	}

}