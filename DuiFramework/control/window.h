#pragma once
#include "base/scoped_ptr.h"
#include "control/control.h"
#include "core/widget.h"
#include "core/focus_manager.h"
#include "core/view.h"

namespace ui
{
	class EventDispatcher;
	class Window 
		: public View
		, public Widget::MessageHandler
		, public FocusManager
	{
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
		

		virtual void SchedulePaintInRect(const Rect& r) override;

		virtual FocusManager* GetFocusManager() const override;

		View* GetFocusedView() const;

		virtual void SetFocus(View* v) override;
	private:
		void SchedulePaint(const Rect& r);
		virtual BOOL ProcessWindowMessage(HWND window,
			UINT message,
			WPARAM w_param,
			LPARAM l_param,
			LRESULT& result) override;

		void ProcessMouseMessage(UINT message, WPARAM w_param, LPARAM l_param);
		void DispatchMouseLeaveEvent(View* from, View* to, const Point& pt);
		void DispatchMouseEnterEvent(View* from, View* to, const Point& pt);

		void ProcessKeyMessage(UINT message, WPARAM w_param, LPARAM l_param);

		void Init();
		Widget* widget() const { return owned_widget_; }
		Widget* owned_widget_{ NULL };

		//scoped_ptr<FocusManager> focus_manager_;
		View* focused_view_{ NULL };
		View* hitttest_view_{ NULL };
	};
}