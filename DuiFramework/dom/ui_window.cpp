#include "stdafx.h"
#include "ui_window.h"
#include "ui_include.h"
#include "render/render_include.h"

namespace ui
{


	UIWindow::UIWindow(UIDocumentPtr doc)
		: UIElement(doc)
	{
		
	}

	UIWindow::~UIWindow()
	{
		owned_widget_->SetMessageHanler(NULL);
		delete owned_widget_;
	}

	void UIWindow::SchedulePaintInRect(const Rect& r)
	{
		if (!r.IsEmpty())
			owned_widget_->InvalidateRect(r);
	}

	BOOL UIWindow::ProcessWindowMessage(HWND window, UINT message, WPARAM w_param, LPARAM l_param, LRESULT& result)
	{
		if (message == WM_PAINT)
		{
			RenderContext painter(owned_widget_);
			DoPaint(&painter, GetLocalBounds());
			return TRUE;
		}
		else if (message == WM_SIZE)
		{
			Size sz = { LOWORD(l_param), HIWORD(l_param) };
			SetSize(sz);
			return TRUE;
		}
		return FALSE;
	}

	void UIWindow::AttatchWidget(Widget* widget)
	{
		owned_widget_ = widget;
		owned_widget_->SetMessageHanler(this);

		owned_widget_->SetSize(Size(styles()->pixelWidth(), styles()->pixelHeight()));

		SetCursor(::LoadCursor(NULL, IDC_ARROW));
	}

	

}