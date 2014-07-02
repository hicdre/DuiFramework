#pragma once
#include "base/ref_counted.h"
#include "render/rect.h"
#include <string>

namespace ui
{
	class Image : public RefCounted<Image>
	{
	public:
		static Image* LoadFromFile(const std::wstring& file_path);

		HBITMAP ToHBITMAP() const;

		int width() const;
		int height() const;
	private:
		friend class RefCounted<Image>;
		Image(HBITMAP bitmap, int w, int h, bool has_alpha);
		~Image();

		HBITMAP bitmap_{ NULL };
		int width_{ 0 };
		int height_{ 0 };
		bool has_aplha_{ false };
	};

	class ImageClip
	{
	public:
		ImageClip(Image* image, const Rect& rect);
		ImageClip(Image* image);
		~ImageClip();

		Rect rect() const { return rect_; }
		Image* image() const { return image_; }
	private:
		Rect rect_;
		Image* image_;
	};
}