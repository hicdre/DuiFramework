#pragma once
#include "render/image_file.h"
#include "core/drawable.h"

namespace ui
{
	class Painter;
	//目前只有拉伸，
	//需要增加平铺等
	class Image : public Drawable
	{
	public:
		Image();
		virtual ~Image();

		void SetImage(ImageRect* image, bool owned = false);

		virtual void DoPaint(Painter* painter, const Rect& dest) override;
		Size GetImageSize() const;
	private:
		void CleanOwnedImage();
		ImageRect* image_{ NULL };
		bool own_image_{ false };
		std::wstring own_image_path_;
	};

	class ResourceImage : public Image
	{
	public:
		ResourceImage();
		ResourceImage(const std::string& id);

		void SetImageId(const std::string& id);
		std::string image_id() const;

	private:
		void LoadImage();
		std::string image_id_;
	};

	class LoadedImage : public Image
	{
	public:
		LoadedImage();
		LoadedImage(const std::wstring& path);

		void SetImagePath(const std::wstring& path);
		std::wstring GetImagePath() const;

	private:
		void LoadImage();
		std::wstring image_path_;
	};
}