#pragma once

namespace ui
{
	class Inseting {
	public:
		Inseting();
		Inseting(int left, int top, int right, int bottom);

		~Inseting() {}

		Inseting& operator=(const Inseting& p);

		void Set(int left, int top, int right, int bottom);

		int left() const { return left_; }
		void set_left(int v) { left_ = v; }

		int top() const { return top_; }
		void set_top(int v) { top_ = v; }

		int right() const { return right_; }
		void set_right(int v) { right_ = v; }

		int bottom() const { return bottom_; }
		void set_bottom(int v) { bottom_ = v; }

		bool IsEmpty() const;
	private:
		int left_{ 0 };
		int top_{ 0 };
		int right_{ 0 };
		int bottom_{ 0 };
	};
}