#pragma once
#include "base/point.h"
#include "base/size.h"
#include "base/inseting.h"

typedef struct tagRECT RECT;

namespace ui
{
	class Rect {
	public:
		Rect();
		Rect(int width, int height);
		Rect(int x, int y, int width, int height);
		explicit Rect(const RECT& r);
		Rect(const Point& origin, const Size& size);

		~Rect() {}

		Rect& operator=(const RECT& r);

		int x() const { return origin_.x(); }
		void set_x(int x) { origin_.set_x(x); }

		int y() const { return origin_.y(); }
		void set_y(int y) { origin_.set_y(y); }

		int width() const { return size_.width(); }
		void set_width(int width);

		int height() const { return size_.height(); }
		void set_height(int height);

		const Point& origin() const { return origin_; }
		void set_origin(const Point& origin) { origin_ = origin; }

		const Size& size() const { return size_; }

		int right() const { return x() + width(); }
		int bottom() const { return y() + height(); }

		void SetRect(int x, int y, int width, int height);
		void SetSize(int width, int height);

		// Shrink the rectangle by a horizontal and vertical distance on all sides.
		void Inset(int horizontal, int vertical) {
			Inset(horizontal, vertical, horizontal, vertical);
		}

		// Shrink the rectangle by the specified amount on each side.
		void Inset(int left, int top, int right, int bottom);
		void Inset(const Inseting& p);

		// Move the rectangle by a horizontal and vertical distance.
		void Offset(int horizontal, int vertical);
		void Offset(const Point& point) {
			Offset(point.x(), point.y());
		}

		// Returns true if the area of the rectangle is zero.
		bool IsEmpty() const { return size_.IsEmpty(); }

		bool operator==(const Rect& other) const;

		bool operator!=(const Rect& other) const {
			return !(*this == other);
		}

		// Construct an equivalent Win32 RECT object.
		RECT ToRECT() const;

		// Returns true if the point identified by point_x and point_y falls inside
		// this rectangle.  The point (x, y) is inside the rectangle, but the
		// point (x + width, y + height) is not.
		bool Contains(int point_x, int point_y) const;

		// Returns true if the specified point is contained by this rectangle.
		bool Contains(const Point& point) const {
			return Contains(point.x(), point.y());
		}

		// Returns true if this rectangle contains the specified rectangle.
		bool Contains(const Rect& rect) const;

		// Returns true if this rectangle intersects the specified rectangle.
		bool Intersects(const Rect& rect) const;

		// Computes the intersection of this rectangle with the given rectangle.
		Rect Intersect(const Rect& rect) const;

		// Computes the union of this rectangle with the given rectangle.  The union
		// is the smallest rectangle containing both rectangles.
		Rect Union(const Rect& rect) const;

		// Computes the rectangle resulting from subtracting |rect| from |this|.  If
		// |rect| does not intersect completely in either the x- or y-direction, then
		// |*this| is returned.  If |rect| contains |this|, then an empty Rect is
		// returned.
		Rect Subtract(const Rect& rect) const;

		// Returns true if this rectangle equals that of the supplied rectangle.
		bool Equals(const Rect& rect) const {
			return *this == rect;
		}

		// Fits as much of the receiving rectangle into the supplied rectangle as
		// possible, returning the result. For example, if the receiver had
		// a x-location of 2 and a width of 4, and the supplied rectangle had
		// an x-location of 0 with a width of 5, the returned rectangle would have
		// an x-location of 1 with a width of 4.
		Rect AdjustToFit(const Rect& rect) const;

		// Returns the center of this rectangle.
		Point CenterPoint() const;

	private:
		Point origin_;
		Size size_;
	};
}