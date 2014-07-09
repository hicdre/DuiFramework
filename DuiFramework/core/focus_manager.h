#pragma once

namespace ui
{
	class WidgetView;
	class View;
	class Widget;
	class FocusManager
	{
	public:
		FocusManager(WidgetView* view);

		View* GetFocusedView() const;

		void SetFocus(View* v);

		void LoseFocus(HWND hwnd);
		void GainFocus(HWND hwnd);
	private:
		WidgetView* view() const { return view_; }
		Widget* widget() const;
		WidgetView* view_;

		View* focused_view_{ NULL };

	};
}