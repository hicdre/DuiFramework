#pragma once
#include "base\basictypes.h"
#include "base\scoped_ptr.h"

namespace ui
{
	class RenderContext;
	class Image;
	/*
		background-color
		background-image: imageid
		background-image-position: x y
		background-image-size: w h
	*/
	class Background
	{
	public:
		Background();
		~Background();

		void SetColor(Color color);
		void SetImage(Image* image);
		void SetPosition(const Point& pt);
		void SetSize(const Size& sz);
		void RestSize();
		void DoPaint(RenderContext* painter, const Rect& dest);
	private:
		Color color_;
		Rect rect_;
		bool is_size_set_{ false };
		scoped_refptr<Image> image_;

	};
}