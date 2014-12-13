#pragma once
#include "base/basictypes.h"

namespace ui
{
	class RenderPath : public RefCounted < RenderPath >
	{
	public:
		virtual ~RenderPath() {}

		virtual void Arc(const Point& center, uint32 radius,
			uint32 angleFrom, uint32 angleTo,
			bool negative = false) = 0;

		virtual void BezierTo(const Point &aCP1,
			const Point &aCP2,
			const Point &aCP3) = 0;

		virtual void QuadraticBezierTo(const Point &aCP1,
			const Point &aCP2) = 0;

		virtual void Rectangle(const Rect& rect, bool negative = false) = 0;

		virtual void RoundRectangle(const RoundRect& rect, bool negative = false) = 0;

		virtual void LineTo(const Point &aPoint) = 0;

		virtual void MoveTo(const Point &aPoint) = 0;

		virtual Point CurrentPoint() const = 0;

		virtual void EndPath() = 0;
	};
}