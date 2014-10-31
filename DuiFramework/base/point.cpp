#include "stdafx.h"
#include "point.h"
#include <windows.h>

namespace ui 
{
	Point::Point() : x_(0), y_(0) {
	}

	Point::Point(int x, int y) : x_(x), y_(y) {
	}

	Point::Point(const POINT& point) : x_(point.x), y_(point.y) {
	}

	Point& Point::operator=(const POINT& point) {
		x_ = point.x;
		y_ = point.y;
		return *this;
	}

	POINT Point::ToPOINT() const {
		POINT p;
		p.x = x_;
		p.y = y_;
		return p;
	}

}  // namespace ui