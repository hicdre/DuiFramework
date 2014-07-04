#pragma once
#include "render/rect.h"

namespace ui
{
	class WidgetView;
	class Widget;
	class WidgetDelegate
	{
	public:
		WidgetDelegate();
		virtual ~WidgetDelegate();

		void Init(WidgetView* view);

		virtual void OnInit() = 0;
		virtual Rect GetInitialRect() = 0;

		WidgetView* view() const { return widget_view_; }
	protected:

		WidgetView* widget_view_{ NULL };
	};
}