#include "stdafx.h"
#include "padding.h"

namespace ui
{

	Padding::Padding()
	{

	}

	Padding::Padding(int left, int top, int right, int bottom)
	{
		SetPadding(left, top, right, bottom);
	}

	Padding& Padding::operator=(const Padding& p)
	{
		SetPadding(p.left_, p.top_, p.right_, p.bottom_);
		return *this;
	}

	void Padding::SetPadding(int left, int top, int right, int bottom)
	{
		left_ = left;
		top_ = top;
		right_ = right;
		bottom_ = bottom;
	}

	bool Padding::IsEmpty() const
	{
		return (left_ == 0) && (top_ == 0) && (right_ == 0) && (bottom_ == 0);
	}

}