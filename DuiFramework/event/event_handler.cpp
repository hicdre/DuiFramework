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
		bool result = HandleMouseMoveOrLeaveEvent(evt);
		return false;
	}

	bool EventHandler::HandleMouseMoveOrLeaveEvent(MouseEvent* evt)
	{
		//widget_->Hittest()
		return false;
	}

}