#include "stdafx.h"
#include "inseting.h"

namespace ui
{

	Inseting::Inseting()
	{

	}

	Inseting::Inseting(int left, int top, int right, int bottom)
	{
		Set(left, top, right, bottom);
	}

	Inseting& Inseting::operator=(const Inseting& p)
	{
		Set(p.left_, p.top_, p.right_, p.bottom_);
		return *this;
	}

	void Inseting::Set(int left, int top, int right, int bottom)
	{
		left_ = left;
		top_ = top;
		right_ = right;
		bottom_ = bottom;
	}

	bool Inseting::IsEmpty() const
	{
		return (left_ == 0) && (top_ == 0) && (right_ == 0) && (bottom_ == 0);
	}

}