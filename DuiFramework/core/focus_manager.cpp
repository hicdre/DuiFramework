#include "stdafx.h"
#include "focus_manager.h"


namespace ui
{

	FocusManager::FocusManager(Window* window)
		: window_(window)
	{

	}

	View* FocusManager::GetFocusedView() const
	{
		return focused_view_;
	}

	void FocusManager::SetFocus(View* v)
	{
		if (focused_view_ == v)
			return;

		View* old_view = focused_view_;
		focused_view_ = v;

#if 0
		if (old_view == NULL)
		{
			Widget* new_widget = focused_view_->GetWidget();
		}

		Widget* old_widget = old_view ? old_view->GetWidget() : NULL;
		Widget* new_widget = focused_view_ ? focused_view_->GetWidget() : NULL;
		if (old_widget != new_widget && new_widget) {
			new_widget->SetFocus();
		}

		if (old_view)
		{
			Event evt(EVENT_FOCUS_OUT, old_view);
			view()->GetEventDispatcher()->DispatchPropagation(&evt, old_view);
		}
		
		if (focused_view_)
		{
			Event evt(EVENT_FOCUS_IN, old_view);
			view()->GetEventDispatcher()->DispatchPropagation(&evt, focused_view_);
		}
#endif
	}

	void FocusManager::LoseFocus(HWND hwnd)
	{
// 		if (hwnd == NULL) {
// 			SetFocus(NULL);
// 			return;
// 		}
// 
// 		if (::GetTopWindow(hwnd) == widget()->hwnd())
// 			return;
// 
// 		SetFocus(NULL);
	}

	void FocusManager::GainFocus(HWND hwnd)
	{
		//SetFocus(view());
	}

}