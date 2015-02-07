#include "stdafx.h"
#include "round_rect.h"

namespace ui
{
	RoundRect::RoundRect(const Rect& rect
		, const Size& topLeft
		, const Size& topRight
		, const Size& bottomRight
		, const Size& bottomLeft)
		: bounds_(rect)
		, topLeft_(topLeft)
		, topRight_(topRight)
		, bottomRight_(bottomRight)
		, bottomLeft_(bottomLeft)
	{

	}

	RoundRect::RoundRect(const Rect& rect
		, int topLeftRadius
		, int topRightRadius
		, int bottomRightRadius
		, int bottomLeftRadius)
		: RoundRect(rect
		, Size(topLeftRadius, topLeftRadius)
		, Size(topRightRadius, topRightRadius)
		, Size(bottomRightRadius, bottomRightRadius)
		, Size(bottomLeftRadius, bottomLeftRadius))
	{

	}

	RoundRect::RoundRect()
	{

	}

	Size RoundRect::GetCornerSize(Corner index) const
	{
		switch (index)
		{
		case TopLeft:
			return topLeft_;
		case TopRight:
			return topRight_;
		case BottomRight:
			return bottomRight_;
		case BottomLeft:
			return bottomLeft_;
		default:
			break;
		}
		assert(0);
		return Size();
	}

	Point RoundRect::AtCorner(Corner index) const
	{
		switch (index)
		{
		case TopLeft:
			return Point(bounds_.x(), bounds_.y());
		case TopRight:
			return Point(bounds_.right(), bounds_.y());
		case BottomRight:
			return Point(bounds_.right(), bounds_.bottom());
		case BottomLeft:
			return Point(bounds_.x(), bounds_.bottom());
		default:
			break;
		}
		assert(0);
		return Point();
	}

	void RoundRect::Inset(const Inseting& inset)
	{
		bounds_.Inset(inset);

		topLeft_.Reduce(inset.left(), inset.top());
		topRight_.Reduce(inset.right(), inset.top());
		bottomRight_.Reduce(inset.right(), inset.bottom());
		bottomLeft_.Reduce(inset.left(), inset.bottom());
	}

}