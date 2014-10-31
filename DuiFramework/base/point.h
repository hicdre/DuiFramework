#pragma once

typedef struct tagPOINT POINT;

namespace ui
{
	//
	// A point has an x and y coordinate.
	//
	class Point {
	public:
		Point();
		Point(int x, int y);
		explicit Point(const POINT& point);
		Point& operator=(const POINT& point);

		~Point() {}

		int x() const { return x_; }
		int y() const { return y_; }

		void SetPoint(int x, int y) {
			x_ = x;
			y_ = y;
		}

		void set_x(int x) { x_ = x; }
		void set_y(int y) { y_ = y; }

		void Offset(int delta_x, int delta_y) {
			x_ += delta_x;
			y_ += delta_y;
		}

		bool operator==(const Point& rhs) const {
			return x_ == rhs.x_ && y_ == rhs.y_;
		}

		bool operator!=(const Point& rhs) const {
			return !(*this == rhs);
		}

		POINT ToPOINT() const;

	private:
		int x_;
		int y_;
	};
}