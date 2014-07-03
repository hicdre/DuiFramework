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

		virtual BOOL ProcessWindowMessage(HWND window,
			UINT message,
			WPARAM w_param,
			LPARAM l_param,
			LRESULT& result) override;

		virtual void SchedulePaintInRect(const Rect& r) override;
	private:
		Rect GetInitialRect();
		Widget* widget() const { return owned_widget_; }
		Widget* owned_widget_{ NULL };

		scoped_ptr<WidgetDelegate> delegate_;
	};
}