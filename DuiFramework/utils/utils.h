#pragma once

#include "render/size.h"

namespace ui
{
	HMODULE GetModuleFromAddress(void* address);

	void CenterAndSizeWindow(HWND parent, HWND window, const Size& pref);
}