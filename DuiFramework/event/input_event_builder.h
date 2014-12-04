#pragma once
#include "event/mouse_event.h"

namespace ui
{
	scoped_refptr<MouseEvent> BuildMouseEvent(HWND hwnd,
		UINT message,
		WPARAM wparam,
		LPARAM lparam,
		DWORD time_ms);
}