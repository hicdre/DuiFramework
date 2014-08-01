#pragma once
#include "core/view.h"
#include "core/widget.h"
#include "core/focus_manager.h"
#include "event/event.h"

namespace ui
{
	class WindowHostView : public View
	{
	public:
		WindowHostView(Window* window);
		virtual ~WindowHostView();

		

	private:		
		virtual void SchedulePaintInRect(const Rect& r) override;
		Window* window_;
	};
}