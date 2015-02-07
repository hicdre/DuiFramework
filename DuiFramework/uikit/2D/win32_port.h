#pragma once
#include "foundation/foundation.h"
#include <windows.h>

namespace ui
{
	Point UICreatePoint(const POINT& point);
	POINT UICreatePOINT(const Point& pt);

	Size UICreateSize(const SIZE& sz);
	SIZE UICreateSIZE(const Size& sz);

	Rect UICreateRect(const RECT& rc);
	RECT UICreateRECT(const Rect& rc);

	XFORM UICreateXFORM(const Matrix& m);
	Matrix UICreateMatrix(const XFORM& xform);

	void CenterAndSizeWindow(HWND parent, HWND window, const Size& pref);
	void AdjustWindowToFit(HWND hwnd, const RECT& bounds, bool fit_to_monitor);
}