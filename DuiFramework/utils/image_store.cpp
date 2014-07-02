#include "stdafx.h"
#include "image_store.h"

#include "core/app.h"

#include <cassert>

namespace ui
{


	ImageStore::ImageStore()
	{

	}

	ImageStore::~ImageStore()
	{
		for (auto iter : ids_clip_map_)
		{
			delete iter.second;
		}

		for (auto iter : image_map_)
		{
			iter.second->Release();
		}
	}


	ImageClip* ImageStore::GetImageById(const std::string& id)
	{
		ImageClip* clip = FindImageClipInCache(id);
		if (clip)
			return clip;

		return LoadImageClipToCache(id);
	}

	ImageClip* ImageStore::FindImageClipInCache(const std::string& id) const
	{
		if (ids_clip_map_.count(id))
			return ids_clip_map_.at(id);
		return NULL;
	}


	ImageClip* ImageStore::LoadImageClipToCache(const std::string& id)
	{
		if (!image_records_.count(id))
			return NULL;

		ImageRecord& record = image_records_.at(id);

		Image* image = FindImageInCache(record.path);
		if (!image) {
			image = LoadImageToCache(record.path);
		}

		if (!image)
			return NULL;

		ImageClip* clip = NULL;
		if (record.rect.IsEmpty()) {
			clip = new ImageClip(image);
		}
		else {
			clip = new ImageClip(image, record.rect);
		}
		ids_clip_map_[id] = clip;
		return clip;
	}
	


	Image* ImageStore::FindImageInCache(const std::wstring& path)
	{
		if (image_map_.count(path))
			return image_map_.at(path);
		return NULL;
	}


	Image* ImageStore::LoadImageToCache(const std::wstring& path)
	{
		Image* image = Image::LoadFromFile(path);
		if (!image) {
			return NULL;
		}
		image_map_[path] = image;
		return image;
	}


	ImageClip* ImageStore::LoadImageByPath(const std::wstring& path, const Rect& rect)
	{
		Image* image = Default()->FindImageInCache(path);
		if (image) {
			return new ImageClip(image, rect);
		}
		return new ImageClip(Image::LoadFromFile(path), rect);
	}

	ImageStore* ImageStore::Default()
	{
		return App::Get()->DefaultImageStore();
	}

	void ImageStore::AddImageRecord(const std::string& id, const std::wstring& path, const Rect& rect)
	{
		ImageRecord record{ path, rect };
		image_records_[id] = std::move(record);
	}

	void ImageStore::AddImageRecord(const std::string& id, const std::wstring& path)
	{
		AddImageRecord(id, path, Rect());
	}

}