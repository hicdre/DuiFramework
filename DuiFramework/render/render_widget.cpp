#include "stdafx.h"
#include "render_widget.h"
#include "render_include.h"
#include "dom/dom_include.h"

namespace ui
{


	RenderWidget::RenderWidget(UIElement* elem)
		: RenderBox(elem)
	{
		owned_widget_ = elem->GetDocument()->GetWidget();
		owned_widget_->SetMessageHanler(this);

		UIStyles* styles = elem->GetRenderStyles();
		owned_widget_->SetSize(Size(styles->pixelWidth(), styles->pixelHeight()));

		SetCursor(::LoadCursor(NULL, IDC_ARROW));
	}

	RenderWidget::~RenderWidget()
	{
		owned_widget_->SetMessageHanler(NULL);
		delete owned_widget_;
	}

	void RenderWidget::DoPaint(RenderContext* painter, const Rect& r)
	{
		RenderBox::DoPaint(painter, r);
	}

	void RenderWidget::Layout()
	{
		RenderBox::Layout();
	}

	BOOL RenderWidget::ProcessWindowMessage(HWND window, UINT message, WPARAM w_param, LPARAM l_param, LRESULT& result)
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

	void RenderWidget::SchedulePaintInRect(const Rect& r)
	{
		//Rect invalid_rect = ConvertRectFromChild(child, r).Intersect(GetLocalBounds());
		if (!r.IsEmpty())
			owned_widget_->InvalidateRect(r);
	}

}