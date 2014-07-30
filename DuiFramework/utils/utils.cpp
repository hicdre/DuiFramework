#include "stdafx.h"
#include "utils.h"

#include "render/rect.h"
#include <cassert>

namespace ui
{
	namespace
	{
		// Adjust the window to fit.
		void AdjustWindowToFit(HWND hwnd, const RECT& bounds, bool fit_to_monitor) 
		{
			if (fit_to_monitor) {
				// Get the monitor.
				HMONITOR hmon = MonitorFromRect(&bounds, MONITOR_DEFAULTTONEAREST);
				if (hmon) {
					MONITORINFO mi;
					mi.cbSize = sizeof(mi);
					GetMonitorInfo(hmon, &mi);
					Rect window_rect(bounds);
					Rect monitor_rect(mi.rcWork);
					Rect new_window_rect = window_rect;
					new_window_rect.AdjustToFit(monitor_rect);
					if (new_window_rect != window_rect) {
						// Window doesn't fit on monitor, move and possibly resize.
						SetWindowPos(hwnd, 0, new_window_rect.x(), new_window_rect.y(),
							new_window_rect.width(), new_window_rect.height(),
							SWP_NOACTIVATE | SWP_NOZORDER);
						return;
					}
					// Else fall through.
				}
				else {
					assert(0); //<< "Unable to find default monitor";
					// Fall through.
				}
			}  // Else fall through.

			// The window is not being fit to monitor, or the window fits on the monitor
			// as is, or we have no monitor info; reset the bounds.
			::SetWindowPos(hwnd, 0, bounds.left, bounds.top,
				bounds.right - bounds.left, bounds.bottom - bounds.top,
				SWP_NOACTIVATE | SWP_NOZORDER);
		}
	}

	HMODULE GetModuleFromAddress(void* address)
	{
		HMODULE instance = NULL;
		if (!::GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
			GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
			static_cast<char*>(address),
			&instance)) {
			assert(0);
		}
		return instance;
	}

	void CenterAndSizeWindow(HWND parent, HWND window, const Size& pref)
	{
		// Calculate the ideal bounds.
		RECT window_bounds;
		RECT center_bounds = { 0 };
		if (parent) {
			// If there is a parent, center over the parents bounds.
			::GetWindowRect(parent, &center_bounds);
		}

		if (::IsRectEmpty(&center_bounds)) {
			// No parent or no parent rect. Center over the monitor the window is on.
			HMONITOR monitor = MonitorFromWindow(window, MONITOR_DEFAULTTONEAREST);
			if (monitor) {
				MONITORINFO mi = { 0 };
				mi.cbSize = sizeof(mi);
				GetMonitorInfo(monitor, &mi);
				center_bounds = mi.rcWork;
			}
			else {
				assert(0);// << "Unable to get default monitor";
			}
		}

		window_bounds.left = center_bounds.left;
		if (pref.width() < (center_bounds.right - center_bounds.left)) {
			window_bounds.left +=
				(center_bounds.right - center_bounds.left - pref.width()) / 2;
		}
		window_bounds.right = window_bounds.left + pref.width();

		window_bounds.top = center_bounds.top;
		if (pref.height() < (center_bounds.bottom - center_bounds.top)) {
			window_bounds.top +=
				(center_bounds.bottom - center_bounds.top - pref.height()) / 2;
		}
		window_bounds.bottom = window_bounds.top + pref.height();

		// If we're centering a child window, we are positioning in client
		// coordinates, and as such we need to offset the target rectangle by the
		// position of the parent window.
		if (::GetWindowLong(window, GWL_STYLE) & WS_CHILD) {
			assert(parent && ::GetParent(window) == parent);
			POINT topleft = { window_bounds.left, window_bounds.top };
			::MapWindowPoints(HWND_DESKTOP, parent, &topleft, 1);
			window_bounds.left = topleft.x;
			window_bounds.top = topleft.y;
			window_bounds.right = window_bounds.left + pref.width();
			window_bounds.bottom = window_bounds.top + pref.height();
		}

		AdjustWindowToFit(window, window_bounds, !parent);
	}

	bool ReadFileToString(const std::wstring& path, std::string* contents)
	{
		if (contents)
			contents->clear();

		HANDLE hFile = CreateFile(path.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == NULL || hFile == INVALID_HANDLE_VALUE)
			return false;

		char buf[1 << 16];
		DWORD len;
		DWORD size = 0;
		bool read_status = true;

		// Many files supplied in |path| have incorrect size (proc files etc).
		// Hence, the file is read sequentially as opposed to a one-shot read.
		while (ReadFile(hFile, buf, sizeof(buf), &len, NULL) && len > 0) {
			if (contents)
				contents->append(buf, len);
			size += len;
		}
		CloseHandle(hFile);

		return true;
	}

	HBITMAP CreateDIB(int width, int height, void** pBits)
	{
		BITMAPINFO bmi = { 0 };
		bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bmi.bmiHeader.biWidth = width;
		bmi.bmiHeader.biHeight = -height;
		bmi.bmiHeader.biPlanes = 1;
		bmi.bmiHeader.biBitCount = 32;
		bmi.bmiHeader.biCompression = BI_RGB;
		bmi.bmiHeader.biSizeImage = width * height * 4;

		bool bAlphaChannel = false;
		if (pBits)
			return ::CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS, pBits, NULL, 0);

		PVOID pIgnore = NULL;
		return ::CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS, &pIgnore, NULL, 0);
	}

	std::wstring MultiByteToWide(const std::string& mb, UINT32 code_page /*= CP_UTF8*/)
	{
		if (mb.empty())
			return std::wstring();

		int mb_length = static_cast<int>(mb.length());
		// Compute the length of the buffer.
		int charcount = MultiByteToWideChar(code_page, 0,
			mb.data(), mb_length, NULL, 0);
		if (charcount == 0)
			return std::wstring();

		std::wstring wide;
		wide.resize(charcount);
		MultiByteToWideChar(code_page, 0, mb.data(), mb_length, &wide[0], charcount);

		return wide;
	}

	std::string WideToMultiByte(const std::wstring& wide, UINT32 code_page /*= CP_UTF8*/)
	{
		int wide_length = static_cast<int>(wide.length());
		if (wide_length == 0)
			return std::string();

		// Compute the length of the buffer we'll need.
		int charcount = WideCharToMultiByte(code_page, 0, wide.data(), wide_length,
			NULL, 0, NULL, NULL);
		if (charcount == 0)
			return std::string();

		std::string mb;
		mb.resize(charcount);
		WideCharToMultiByte(code_page, 0, wide.data(), wide_length,
			&mb[0], charcount, NULL, NULL);

		return mb;
	}

}