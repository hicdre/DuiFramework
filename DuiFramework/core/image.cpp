#include "stdafx.h"
#include "image.h"

#include "render/painter.h"
#include "utils/image_store.h"

namespace ui
{

	Image::Image()
	{
	}

	Image::~Image()
	{
		CleanOwnedImage();
	}


	void Image::SetImage(ImageRect* image, bool owned /*= false*/)
	{
		if (image_ == image) {
			assert(!owned);
			return;
		}

		CleanOwnedImage();
		
		image_ = image;
		own_image_ = owned;
	}

	void Image::DoPaint(Painter* painter, const Rect& dest)
	{
		if (!image_)
			return;
		painter->DrawImage(image_, dest);
	}

	void Image::CleanOwnedImage()
	{
		if (own_image_ && image_)
		{
			delete image_;
			image_ = NULL;
		}
		own_image_ = false;
	}

	Size Image::GetImageSize() const
	{
		if (!image_)
			return Size();
		return Size(image_->rect().width(), image_->rect().height());
	}


	ResourceImage::ResourceImage()
	{

	}

	ResourceImage::ResourceImage(const std::string& id)
		: image_id_(id)
	{
		LoadImage();
	}

	void ResourceImage::SetImageId(const std::string& id)
	{
		if (image_id_ == id)
			return;
		LoadImage();
	}

	std::string ResourceImage::image_id() const
	{
		return image_id_;
	}

	void ResourceImage::LoadImage()
	{
		ImageRect* clip = ImageStore::Default()->GetImageById(image_id_);
		SetImage(clip);
	}


	LoadedImage::LoadedImage()
	{

	}

	LoadedImage::LoadedImage(const std::wstring& path)
		: image_path_(path)
	{
		LoadImage();
	}

	void LoadedImage::SetImagePath(const std::wstring& path)
	{
		if (path == image_path_)
			return;

		image_path_ = path;
		LoadImage();
	}

	std::wstring LoadedImage::GetImagePath() const
	{
		return image_path_;
	}

	void LoadedImage::LoadImage()
	{
		ImageRect* clip = ImageStore::LoadImageByPath(image_path_);
		SetImage(clip, true);
	}

}