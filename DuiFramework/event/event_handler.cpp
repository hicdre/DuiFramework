#include "stdafx.h"
#include "event_handler.h"
#include "event_include.h"

namespace ui
{


	EventHandler::EventHandler(RenderWidget* widget)
		: widget_(widget)
	{

	}

	EventHandler::~EventHandler()
	{

	}

	bool EventHandler::HandleMouseMoveEvent(MouseEvent* evt)
	{
		bool result = HandleMouseMoveOrLeaveEvent(evt, &hoveredNode);
	}

	bool EventHandler::HandleMouseMoveOrLeaveEvent(MouseEvent* evt)
	{
		//widget_->Hittest()
	}

}