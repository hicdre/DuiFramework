#pragma once
#include "base/ref_counted.h"
#include "render/rect.h"
#include <string>

namespace ui
{
	class ImageFile : public RefCounted<ImageFile>
	{
	public:
		static ImageFile* LoadFromFile(const std::wstring& file_path);

		HBITMAP ToHBITMAP() const;

		int width() const;
		int height() const;
	private:
		friend class RefCounted<ImageFile>;
		ImageFile(HBITMAP bitmap, int w, int h, bool has_alpha);
		~ImageFile();

		HBITMAP bitmap_{ NULL };
		int width_{ 0 };
		int height_{ 0 };
		bool has_aplha_{ false };
	};

	class ImageRect
	{
	public:
		ImageRect(ImageFile* image, const Rect& rect);
		ImageRect(ImageFile* image);
		~ImageRect();

		Rect rect() const { return rect_; }
		ImageFile* image() const { return image_; }
	private:
		Rect rect_;
		ImageFile* image_;
	};
}