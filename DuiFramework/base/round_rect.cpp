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

}