#pragma once
#include "base/rect.h"

namespace ui
{
	class RoundRect
	{
	public:
		RoundRect(const Rect& rect
			, const Size& topLeft
			, const Size& topRight
			, const Size& bottomRight
			, const Size& bottomLeft);



	private:
		Rect bounds_;
		Size topLeft_;
		Size topRight_;
		Size bottomRight_;
		Size bottomLeft_;
	};
}