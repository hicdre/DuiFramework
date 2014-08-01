#pragma once
#include "base/scoped_ptr.h"
#include "control/control.h"
#include "core/widget.h"
#include "core/focus_manager.h"

namespace ui
{
	class EventDispatcher;
	class Window : public Control, public Widget::MessageHandler
	{
		friend class WindowHostView;
		friend class MouseEventHandler;
	public:
		Window();
		virtual ~Window();

		void SetBounds(int x, int y, int width, int height);
		Rect GetBounds();
		void SetSize(int w, int h);
		Size GetSize();

		void CenterWindow();

		void Close();

		virtual void OnSetFocus();
		

		//virtual void SchedulePaintInRect(const Rect& r) override;

		View* GetFocusedView() const;
	private:
		void SchedulePaint(const Rect& r);
		virtual BOOL ProcessWindowMessage(HWND window,
			UINT message,
			WPARAM w_param,
			LPARAM l_param,
			LRESULT& result) override;

		void Init();
		Widget* widget() const { return owned_widget_; }
		Widget* owned_widget_{ NULL };

		scoped_ptr<MouseEventHandler> mouse_event_handler_;
		//scoped_ptr<FocusManager> focus_manager_;
		View* focused_view_;
		View* hittest_view_;
		Point mouse_position_;
	};
}