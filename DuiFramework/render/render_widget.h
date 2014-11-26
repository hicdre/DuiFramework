#pragma once
#include "render/render_box.h"
#include "core/widget.h"

namespace ui
{
	class RenderWidget 
		: public RenderBox
		, public Widget::MessageHandler
	{
	public:
		RenderWidget(UIElement* elem);
		virtual ~RenderWidget();

		virtual void DoPaint(RenderContext* painter, const Rect& r) override;
		virtual void Layout() override;

		virtual void SchedulePaintInRect(const Rect& r) override;

		virtual BOOL ProcessWindowMessage(HWND window,
			UINT message,
			WPARAM w_param,
			LPARAM l_param,
			LRESULT& result) override;

	private:
		Widget* owned_widget_{ NULL };
	};
}