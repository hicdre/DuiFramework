#include "stdafx.h"
#include "win32_port.h"
#include "uikit/common/ui_font.h"

namespace ui
{

	Point UICreatePoint(const POINT& point)
	{
		return Point(point.x, point.y);
	}

	POINT UICreatePOINT(const Point& pt)
	{
		POINT p = { pt.x(), pt.y() };
		return p;
	}

	Size UICreateSize(const SIZE& sz)
	{
		return Size(sz.cx, sz.cy);
	}

	SIZE UICreateSIZE(const Size& sz)
	{
		SIZE s = { sz.width(), sz.height() };
		return s;
	}

	Rect UICreateRect(const RECT& rc)
	{
		return Rect(rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top);
	}

	RECT UICreateRECT(const Rect& rc)
	{
		RECT r;
		r.left = rc.x();
		r.right = rc.right();
		r.top = rc.y();
		r.bottom = rc.bottom();
		return r;
	}

	XFORM UICreateXFORM(const Matrix& m)
	{
		XFORM x = { m.a, m.b, m.c, m.d, m.tx, m.ty };
		return x;
	}

	Matrix UICreateMatrix(const XFORM& xform)
	{
		Matrix m;
		m.a = xform.eM11;
		m.b = xform.eM12;
		m.c = xform.eM21;
		m.d = xform.eM22;
		m.tx = xform.eDx;
		m.ty = xform.eDy;
		return m;
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

	void AdjustWindowToFit(HWND hwnd, const RECT& bounds, bool fit_to_monitor)
	{
		if (fit_to_monitor) {
			// Get the monitor.
			HMONITOR hmon = MonitorFromRect(&bounds, MONITOR_DEFAULTTONEAREST);
			if (hmon) {
				MONITORINFO mi;
				mi.cbSize = sizeof(mi);
				GetMonitorInfo(hmon, &mi);
				Rect window_rect = UICreateRect(bounds);
				Rect monitor_rect = UICreateRect(mi.rcWork);
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