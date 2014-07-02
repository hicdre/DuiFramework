#pragma once
#include "render/rect.h"
#include "render/image.h"

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
		
		//返回的ImageClip*无需delete
		ImageClip* GetImageById(const std::string& id);
		ImageClip* FindImageClipInCache(const std::string& id) const;
		ImageClip* LoadImageClipToCache(const std::string& id);

		static ImageStore* Default();
		//返回的ImageClip*需要delete
		static ImageClip* LoadImageByPath(const std::wstring& path, const Rect& rect);
	private:
		Image* FindImageInCache(const std::wstring& path);
		Image* LoadImageToCache(const std::wstring& path);
		std::unordered_map<std::string, ImageClip*> ids_clip_map_;
		std::unordered_map<std::wstring, Image*> image_map_;

		struct ImageRecord
		{
			std::wstring path;
			Rect rect;
		};
		std::unordered_map<std::string, ImageRecord> image_records_;
	};
}