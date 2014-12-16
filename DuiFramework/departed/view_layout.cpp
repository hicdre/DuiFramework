#include "stdafx.h"
#include "view_layout.h"

namespace ui
{



	LayoutBox::LayoutBox()
	{

	}

	LayoutBox::~LayoutBox()
	{

	}

	const Length& LayoutBox::x()
	{
		return margin_left_;
	}

	const Length& LayoutBox::y()
	{
		return margin_top_;
	}

	const Length& LayoutBox::width()
	{
		return width_;
	}

	const Length& LayoutBox::height()
	{
		return height_;
	}

	const Length& LayoutBox::marginLeft()
	{
		return margin_left_;
	}

	const Length& LayoutBox::marginTop()
	{
		return margin_top_;
	}

	const Length& LayoutBox::marginRight()
	{
		return margin_right_;
	}

	const Length& LayoutBox::marginBottom()
	{
		return margin_bottom_;
	}

	void LayoutBox::setX(const Length& l)
	{
		margin_left_ = l;
	}

	void LayoutBox::setY(const Length& l)
	{
		margin_top_ = l;
	}

	void LayoutBox::setWidth(const Length& l)
	{
		width_ = l;
	}

	void LayoutBox::setHeight(const Length& l)
	{
		height_ = l;
	}

	void LayoutBox::setMarginLeft(const Length& l)
	{
		margin_left_ = l;
	}

	void LayoutBox::setMarginTop(const Length& l)
	{
		margin_top_ = l;
	}

	void LayoutBox::setMarginRight(const Length& l)
	{
		margin_right_ = l;
	}

	void LayoutBox::setMarginBottom(const Length& l)
	{
		margin_bottom_ = l;
	}

}