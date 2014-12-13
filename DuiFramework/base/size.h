#pragma once
#include "base/base_defines.h"

typedef struct tagSIZE SIZE;

namespace ui
{
	//
	// A size has width and height values.
	//
	class Size {
	public:
		Size() : width_(0), height_(0) {}
		Size(int width, int height);

		~Size() {}

		int width() const { return width_; }
		int height() const { return height_; }

		int GetArea() const { return width_ * height_; }

		void SetSize(int width, int height) {
			set_width(width);
			set_height(height);
		}

		void Enlarge(int width, int height) {
			set_width(width_ + width);
			set_height(height_ + height);
		}

		void Reduce(uint32 width, uint32 height) {
			if (width_ - width > 0)
				set_width(width_ - width);
			else
				set_width(0);

			if (height_ - height > 0)
				set_height(height_ - height);
			else
				set_height(0);
		}

		void set_width(int width);
		void set_height(int height);

		bool operator==(const Size& s) const {
			return width_ == s.width_ && height_ == s.height_;
		}

		bool operator!=(const Size& s) const {
			return !(*this == s);
		}

		bool IsEmpty() const {
			// Size doesn't allow negative dimensions, so testing for 0 is enough.
			return (width_ == 0) || (height_ == 0);
		}

		SIZE ToSIZE() const;

	private:
		int width_;
		int height_;
	};
}