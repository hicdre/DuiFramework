#include "stdafx.h"
#include "render_widget.h"
#include "render_include.h"
#include "dom/dom_include.h"

namespace ui
{


	RenderWidget::RenderWidget(UIElement* elem)
		: RenderObject(elem)
	{
		owned_widget_ = elem->GetDocument()->GetWidget();
		owned_widget_->SetMessageHanler(this);

		RenderStyles* styles = elem->GetRenderStyles();
		int ml = styles->marginLeft();

	}

	RenderWidget::~RenderWidget()
	{
		owned_widget_->SetMessageHanler(NULL);
		delete owned_widget_;
	}

	void RenderWidget::DoPaint(RenderContext* painter, const Rect& r)
	{

	}

	void RenderWidget::Layout()
	{

	}

	BOOL RenderWidget::ProcessWindowMessage(HWND window, UINT message, WPARAM w_param, LPARAM l_param, LRESULT& result)
	{
		if (message == WM_PAINT)
		{
			RenderContext painter(owned_widget_);
			DoPaint(&painter, GetLocalBounds());
			return TRUE;
		}
		return FALSE;
	}

}