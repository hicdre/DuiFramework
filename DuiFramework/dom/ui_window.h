#pragma once
#include "dom/ui_element.h"
#include "core/widget.h"
#include "event/event_handler.h"

namespace ui
{
	class UIWindow 
		: public UIElement
		, public Widget::MessageHandler
	{
	public:
		UIWindow(UIDocumentPtr doc);
		~UIWindow();

		void AttatchWidget(Widget* widget);

		void SetCursor(CursorId id);

		virtual void SchedulePaintInRect(const Rect& r) override;
		virtual BOOL ProcessWindowMessage(HWND window,
			UINT message,
			WPARAM w_param,
			LPARAM l_param,
			LRESULT& result) override;

	private:
		Widget* owned_widget_{ NULL };

		EventHandler event_handler_;
	};
}