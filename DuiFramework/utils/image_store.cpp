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


	ImagePart* ImageStore::GetImageById(const std::string& id)
	{
		ImagePart* clip = FindImageClipInCache(id);
		if (clip)
			return clip;

		return LoadImageClipToCache(id);
	}

	ImagePart* ImageStore::FindImageClipInCache(const std::string& id) const
	{
		if (ids_clip_map_.count(id))
			return ids_clip_map_.at(id);
		return NULL;
	}


	ImagePart* ImageStore::LoadImageClipToCache(const std::string& id)
	{
		if (!image_records_.count(id))
			return NULL;

		ImageRecord& record = image_records_.at(id);

		ImageFile* image = FindImageInCache(record.path);
		if (!image) {
			image = LoadImageToCache(record.path);
		}

		if (!image)
			return NULL;

		ImagePart* clip = NULL;
		if (record.rect.IsEmpty()) {
			clip = new ImagePart(image);
		}
		else {
			clip = new ImagePart(image, record.rect);
		}
		ids_clip_map_[id] = clip;
		return clip;
	}
	


	ImageFile* ImageStore::FindImageInCache(const std::wstring& path)
	{
		if (image_map_.count(path))
			return image_map_.at(path);
		return NULL;
	}


	ImageFile* ImageStore::LoadImageToCache(const std::wstring& path)
	{
		ImageFile* image = ImageFile::LoadFromFile(path);
		if (!image) {
			return NULL;
		}
		image->AddRef();
		image_map_[path] = image;
		return image;
	}


	ImagePart* ImageStore::LoadImageByPath(const std::wstring& path, const Rect& rect)
	{
		ImageFile* image = Default()->FindImageInCache(path);
		if (image) {
			return new ImagePart(image, rect);
		}
		return new ImagePart(ImageFile::LoadFromFile(path), rect);
	}

	ImagePart* ImageStore::LoadImageByPath(const std::wstring& path)
	{
		ImageFile* image = Default()->FindImageInCache(path);
		if (image) {
			return new ImagePart(image);
		}
		return new ImagePart(ImageFile::LoadFromFile(path));
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