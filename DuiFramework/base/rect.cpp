#include "stdafx.h"
#include "rect.h"
#include <algorithm>

namespace {

	void AdjustAlongAxis(int dst_origin, int dst_size, int* origin, int* size) {
		if (*origin < dst_origin) {
			*origin = dst_origin;
			*size = (std::min)(dst_size, *size);
		}
		else {
			*size = (std::min)(dst_size, *size);
			*origin = (std::min)(dst_origin + dst_size, *origin + *size) - *size;
		}
	}
}

namespace ui
{
	Rect::Rect() {
	}

	Rect::Rect(int width, int height) {
		set_width(width);
		set_height(height);
	}

	Rect::Rect(int x, int y, int width, int height)
		: origin_(x, y) {
		set_width(width);
		set_height(height);
	}

	Rect::Rect(const Point& origin, const Size& size)
		: origin_(origin), size_(size) {
	}

	Rect::Rect(const RECT& r)
		: origin_(r.left, r.top) {
		set_width(r.right - r.left);
		set_height(r.bottom - r.top);
	}

	Rect& Rect::operator=(const RECT& r) {
		origin_.SetPoint(r.left, r.top);
		set_width(r.right - r.left);
		set_height(r.bottom - r.top);
		return *this;
	}

	void Rect::set_width(int width) {
		size_.set_width(width);
	}
	void Rect::set_height(int height) {
		size_.set_height(height);
	}

	void Rect::SetRect(int x, int y, int width, int height) {
		origin_.SetPoint(x, y);
		set_width(width);
		set_height(height);
	}

	void Rect::Inset(int left, int top, int right, int bottom) {
		Offset(left, top);
		set_width((std::max)(width() - left - right, 0));
		set_height((std::max)(height() - top - bottom, 0));
	}

	void Rect::Inset(const Inseting& p)
	{
		Inset(p.left(), p.top(), p.right(), p.bottom());
	}

	void Rect::Offset(int horizontal, int vertical) {
		set_x(x() + horizontal);
		set_y(y() + vertical);
	}

	bool Rect::operator==(const Rect& other) const {
		return origin_ == other.origin_ && size_ == other.size_;
	}

	RECT Rect::ToRECT() const {
		RECT r;
		r.left = x();
		r.right = right();
		r.top = y();
		r.bottom = bottom();
		return r;
	}

	bool Rect::Contains(int point_x, int point_y) const {
		return (point_x >= x()) && (point_x < right()) &&
			(point_y >= y()) && (point_y < bottom());
	}

	bool Rect::Contains(const Rect& rect) const {
		return (rect.x() >= x() && rect.right() <= right() &&
			rect.y() >= y() && rect.bottom() <= bottom());
	}

	bool Rect::Intersects(const Rect& rect) const {
		return !(rect.x() >= right() || rect.right() <= x() ||
			rect.y() >= bottom() || rect.bottom() <= y());
	}

	Rect Rect::Intersect(const Rect& rect) const {
		int rx = (std::max)(x(), rect.x());
		int ry = (std::max)(y(), rect.y());
		int rr = (std::min)(right(), rect.right());
		int rb = (std::min)(bottom(), rect.bottom());

		if (rx >= rr || ry >= rb)
			rx = ry = rr = rb = 0;  // non-intersecting

		return Rect(rx, ry, rr - rx, rb - ry);
	}

	Rect Rect::Union(const Rect& rect) const {
		// special case empty rects...
		if (IsEmpty())
			return rect;
		if (rect.IsEmpty())
			return *this;

		int rx = (std::min)(x(), rect.x());
		int ry = (std::min)(y(), rect.y());
		int rr = (std::max)(right(), rect.right());
		int rb = (std::max)(bottom(), rect.bottom());

		return Rect(rx, ry, rr - rx, rb - ry);
	}

	Rect Rect::Subtract(const Rect& rect) const {
		// boundary cases:
		if (!Intersects(rect))
			return *this;
		if (rect.Contains(*this))
			return Rect();

		int rx = x();
		int ry = y();
		int rr = right();
		int rb = bottom();

		if (rect.y() <= y() && rect.bottom() >= bottom()) {
			// complete intersection in the y-direction
			if (rect.x() <= x()) {
				rx = rect.right();
			}
			else {
				rr = rect.x();
			}
		}
		else if (rect.x() <= x() && rect.right() >= right()) {
			// complete intersection in the x-direction
			if (rect.y() <= y()) {
				ry = rect.bottom();
			}
			else {
				rb = rect.y();
			}
		}
		return Rect(rx, ry, rr - rx, rb - ry);
	}

	Rect Rect::AdjustToFit(const Rect& rect) const {
		int new_x = x();
		int new_y = y();
		int new_width = width();
		int new_height = height();
		AdjustAlongAxis(rect.x(), rect.width(), &new_x, &new_width);
		AdjustAlongAxis(rect.y(), rect.height(), &new_y, &new_height);
		return Rect(new_x, new_y, new_width, new_height);
	}

	Point Rect::CenterPoint() const {
		return Point(x() + (width() + 1) / 2, y() + (height() + 1) / 2);
	}

	void Rect::SetSize(int width, int height)
	{
		size_.SetSize(width, height);
	}

}