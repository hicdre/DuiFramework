#pragma once

#include "base\basictypes.h"

#include <string>
#include <vector>

namespace ui
{
	HMODULE GetModuleFromAddress(void* address);

	void CenterAndSizeWindow(HWND parent, HWND window, const Size& pref);

	bool ReadFileToString(const std::wstring& path,	std::string* contents);

	//创建出的坐标系与屏幕坐标系一致
	HBITMAP CreateDIB(int width, int height, void** pBits = NULL);

	std::wstring MultiByteToWide(const std::string& mb, UINT32 code_page = CP_UTF8);
	std::string WideToMultiByte(const std::wstring& wide, UINT32 code_page = CP_UTF8);

	void spilt_string(const std::string& input, char tag, std::vector<std::string>& output, size_t maxcount = static_cast<size_t>(-1));

	std::wstring dirname(const std::wstring& path);

	bool isdir(const std::wstring& path);

	bool mkdir(const std::wstring& path);

	bool rm(const std::wstring& path, bool recursive);

	std::wstring pathcombine(const std::wstring& dir, const wchar_t* file);
	std::wstring pathcombine(const std::wstring& dir, const std::wstring& file);
}