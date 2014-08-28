#include "stdafx.h"
#include "image_file.h"

#include "third_party/stb_image.h"
#include "utils/utils.h"

namespace ui
{

	ImageFile::ImageFile(HBITMAP bitmap, int w, int h, bool has_alpha)
		: bitmap_(bitmap), width_(w), height_(h), has_aplha_(has_alpha)
	{

	}

	ImageFile::~ImageFile()
	{
		::DeleteObject(bitmap_);
	}

	ImageFile* ImageFile::LoadFromFile(const std::wstring& file_path)
	{
		std::string buffer;
		if (!ReadFileToString(file_path, &buffer))
			return NULL;

		int x, y, n;
		stbi_uc* pBuffer = stbi_load_from_memory((stbi_uc*)buffer.c_str(), buffer.size(), &x, &y, &n, 4);
		if (!pBuffer)
			return NULL;

		LPBYTE pBits = NULL;
		HBITMAP hbitmap = CreateDIB(x, y, (void**)&pBits);
		if (!hbitmap) {
			stbi_image_free(pBuffer);
			return NULL;
		}

		bool has_alpha_channel = false;
		for (int i = 0; i < x * y; i++)
		{
			pBits[i * 4 + 3] = pBuffer[i * 4 + 3];
			if (pBits[i * 4 + 3] < 255)
			{
				pBits[i * 4] = (BYTE)(DWORD(pBuffer[i * 4 + 2])*pBuffer[i * 4 + 3] / 255);
				pBits[i * 4 + 1] = (BYTE)(DWORD(pBuffer[i * 4 + 1])*pBuffer[i * 4 + 3] / 255);
				pBits[i * 4 + 2] = (BYTE)(DWORD(pBuffer[i * 4])*pBuffer[i * 4 + 3] / 255);
				has_alpha_channel = true;
			}
			else
			{
				pBits[i * 4] = pBuffer[i * 4 + 2];
				pBits[i * 4 + 1] = pBuffer[i * 4 + 1];
				pBits[i * 4 + 2] = pBuffer[i * 4];
			}
		}

		stbi_image_free(pBuffer);

		ImageFile* ret = new ImageFile(hbitmap, x, y, has_alpha_channel);
		return ret;
	}

	HBITMAP ImageFile::ToHBITMAP() const
	{
		return bitmap_;
	}

	int ImageFile::width() const
	{
		return width_;
	}

	int ImageFile::height() const
	{
		return height_;
	}

	


	ImageRect::ImageRect(ImageFile* image, const Rect& rect)
		: image_(image), rect_(rect)
	{
		image_->AddRef();
	}

	ImageRect::ImageRect(ImageFile* image)
		: image_(image), rect_(image->width(), image->height())
	{
		image_->AddRef();
	}

	ImageRect::~ImageRect()
	{
		image_->Release();
	}

}