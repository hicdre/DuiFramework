#pragma once
#include "event_forward.h"

namespace ui
{
	class RenderWidget;
	class EventHandler
	{
	public:
		explicit EventHandler(RenderWidget* widget);
		~EventHandler();

		bool HandleMouseMoveEvent(MouseEvent* evt);
		bool HandleMouseMoveOrLeaveEvent(MouseEvent* evt);

	private:
		RenderWidget* widget_;
	};
}