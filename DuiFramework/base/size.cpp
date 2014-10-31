#include "stdafx.h"
#include "size.h"

namespace ui
{
	Size::Size(int width, int height) {
		set_width(width);
		set_height(height);
	}

	SIZE Size::ToSIZE() const {
		SIZE s;
		s.cx = width_;
		s.cy = height_;
		return s;
	}

	void Size::set_width(int width) {
		if (width < 0) {
			//NOTREACHED();
			width = 0;
		}
		width_ = width;
	}

	void Size::set_height(int height) {
		if (height < 0) {
			//NOTREACHED();
			height = 0;
		}
		height_ = height;
	}
}