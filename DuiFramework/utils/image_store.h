#pragma once
#include "render/rect.h"
#include "render/image_file.h"

#include <string>
#include <unordered_map>

namespace ui
{
	class ImageStore
	{
	public:
		ImageStore();
		~ImageStore();

		void AddImageRecord(const std::string& id, const std::wstring& path, const Rect& rect);
		void AddImageRecord(const std::string& id, const std::wstring& path);
		
		//返回的ImageRect*无需delete
		ImageRect* GetImageById(const std::string& id);
		ImageRect* FindImageClipInCache(const std::string& id) const;
		ImageRect* LoadImageClipToCache(const std::string& id);

		static ImageStore* Default();
		//返回的ImageClip*需要delete
		static ImageRect* LoadImageByPath(const std::wstring& path, const Rect& rect);
		static ImageRect* LoadImageByPath(const std::wstring& path);
	private:
		ImageFile* FindImageInCache(const std::wstring& path);
		ImageFile* LoadImageToCache(const std::wstring& path);
		std::unordered_map<std::string, ImageRect*> ids_clip_map_;
		std::unordered_map<std::wstring, ImageFile*> image_map_;

		struct ImageRecord
		{
			std::wstring path;
			Rect rect;
		};
		std::unordered_map<std::string, ImageRecord> image_records_;
	};
}