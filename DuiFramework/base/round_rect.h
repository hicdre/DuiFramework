#pragma once
#include "base/rect.h"

namespace ui
{
	class RoundRect
	{
	public:
		enum Corner{
			TopLeft = 0,
			TopRight,
			BottomRight,
			BottomLeft,
		};
		RoundRect();

		RoundRect(const Rect& rect
			, const Size& topLeft
			, const Size& topRight
			, const Size& bottomRight
			, const Size& bottomLeft);

		RoundRect(const Rect& rect
			, int topLeftRadius
			, int topRightRadius
			, int bottomRightRadius
			, int bottomLeftRadius);

		int x() const { return bounds_.x(); }
		int y() const { return bounds_.y(); }
		int right() const { return bounds_.right(); }
		int bottom() const { return bounds_.bottom(); }

		int width() const { return bounds_.width(); }
		int height() const { return bounds_.height(); }

		Size GetCornerSize(Corner index) const;
		Point AtCorner(Corner index) const;

		void Inset(const Inseting& inset);
	private:
		Rect bounds_;
		Size topLeft_;
		Size topRight_;
		Size bottomRight_;
		Size bottomLeft_;
	};
}