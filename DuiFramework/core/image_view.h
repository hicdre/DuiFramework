#pragma once
#include "core/view.h"

namespace ui
{
	//目前只有拉伸，
	//需要增加平铺等
	class ImageView : public View
	{
	public:
		ImageView();
		virtual ~ImageView() override;

		void SetImage(ImageClip* image, bool owned = false);

		virtual void OnPaint(Painter* painter) override;
		virtual Size GetPreferredSize() const override;
	private:
		void CleanOwnedImage();
		ImageClip* image_{ NULL };
		bool own_image_{ false };
		std::wstring own_image_path_;
	};

	class ResourceImage : public ImageView
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

	class LoadedImage : public ImageView
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