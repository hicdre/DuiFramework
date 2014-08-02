#pragma once

namespace ui
{
	class Window;
	class View;
	class Widget;
	class FocusManager
	{
	public:
		FocusManager(Window* window);

		View* GetFocusedView() const;

		void SetFocus(View* v);

		void LoseFocus(HWND hwnd);
		void GainFocus(HWND hwnd);
	private:
		Window* window_;

		View* focused_view_{ NULL };

	};
}