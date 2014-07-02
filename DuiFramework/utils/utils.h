#pragma once

#include "render/size.h"

#include <string>

namespace ui
{
	HMODULE GetModuleFromAddress(void* address);

	void CenterAndSizeWindow(HWND parent, HWND window, const Size& pref);

	bool ReadFileToString(const std::wstring& path,	std::string* contents);

	//创建出的坐标系与屏幕坐标系一致
	HBITMAP CreateDIB(int width, int height, void** pBits);
}