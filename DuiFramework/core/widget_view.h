#pragma once
#include "core/view.h"
#include "core/widget.h"
#include "core/widget_delegate.h"

namespace ui
{
	class WidgetView : public View, public Widget::MessageHandler
	{
	public:
		WidgetView(WidgetDelegate* delegate);
		virtual ~WidgetView();

		static Widget* CreateWidget(WidgetDelegate* delegate);

		virtual const Widget* GetWidget() const override;
		void Init();

		void CenterWidget();

		virtual BOOL ProcessWindowMessage(HWND window,
			UINT message,
			WPARAM w_param,
			LPARAM l_param,
			LRESULT& result) override;

		virtual void SchedulePaintInRect(const Rect& r) override;
	private:
		LRESULT HandleMouseEvent(UINT message,
			WPARAM w_param,
			LPARAM l_param);

		void UpdateMousePosition(const Point& new_pos);

		void DispatchMouseLeaveEvent(View* from, View* to);
		void DispatchMouseEnterEvent(View* from, View* to);
		void DispatchMouseMoveEvent(View* from);

		Rect GetInitialRect();
		Widget* widget() const { return owned_widget_; }
		Widget* owned_widget_{ NULL };

		MSG msg_;
		View* hitttest_view_{ NULL };
		Point mouse_position_;

		scoped_ptr<WidgetDelegate> delegate_;
	};
}