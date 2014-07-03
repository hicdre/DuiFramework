#include "stdafx.h"
#include "widget_view.h"

namespace ui
{

	Widget* WidgetView::CreateWidget(WidgetDelegate* delegate)
	{
		WidgetView* view = new WidgetView(delegate);
		view->Init();
		return const_cast<Widget*>(view->GetWidget());
		//ui::Widget* widget = ui::Widget::Create(NULL, delegate->GetInitialRect());
		//widget->SetView(new WidgetView(delegate));
	}

	WidgetView::WidgetView(WidgetDelegate* delegate)
		: delegate_(delegate)
	{

	}

	WidgetView::~WidgetView()
	{

	}

	void WidgetView::Init()
	{
		SetCursor(::LoadCursor(NULL, IDC_ARROW));

		owned_widget_ = ui::Widget::Create(NULL, GetInitialRect());
		owned_widget_->SetMessageHanler(this);
		Rect view_rect = owned_widget_->GetWindowScreenBounds();
		SetBounds(0, 0, view_rect.width(), view_rect.height());

		delegate_->Init(this);
	}

	Rect WidgetView::GetInitialRect()
	{
		return delegate_->GetInitialRect();
	}

	const Widget* WidgetView::GetWidget() const
	{
		return owned_widget_;
	}

	BOOL WidgetView::ProcessWindowMessage(HWND window, UINT message, WPARAM w_param, LPARAM l_param, LRESULT& result)
	{
		if (message == WM_PAINT)
		{
			Painter painter(owned_widget_);
			DoPaint(&painter);
			return TRUE;
		}
		else if (message == WM_MOUSEMOVE)
		{
			Point pt(((int)(short)LOWORD(l_param)), ((int)(short)HIWORD(l_param)));
			View* v = Hittest(pt);
			HCURSOR cursor = v->GetCursor();
			widget()->SetCursor(cursor);

		}
		return FALSE;
	}

	void WidgetView::SchedulePaintInRect(const Rect& r)
	{
		if (!visible_ || !painting_enabled_)
			return;

		Rect invalid_rect(GetLocalBounds());
		invalid_rect.Intersect(r);
		if (!invalid_rect.IsEmpty())
			widget()->InvalidateRect(invalid_rect);
	}

}