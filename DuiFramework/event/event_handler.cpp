#include "stdafx.h"
#include "event_handler.h"
#include "event_include.h"
#include "dom/ui_window.h"

namespace ui
{


	EventHandler::EventHandler(UIWindow* window)
		: window_(window)
	{

	}

	EventHandler::~EventHandler()
	{

	}

	bool EventHandler::HandleMouseMoveEvent(MouseEvent* evt)
	{
		HitTestResult hitResult;
		bool result = HandleMouseMoveOrLeaveEvent(evt, &hitResult);
		return false;
	}

	bool EventHandler::HandleMouseMoveOrLeaveEvent(MouseEvent* evt, HitTestResult* result)
	{
		if (!window_->Hittest(result, evt->clientLocation())) {
			assert(0);
			return false;
		}

		//设置cursor


		DispatchMouseEvent(EVENT_MOUSE_MOVE, result->element(), 0, evt, true);


		return false;
	}

	bool EventHandler::DispatchMouseEvent(EventType type, UIElement* target, int clickCount, MouseEvent* evt, bool setUnder)
	{
		UpdateMouseEventTarget(target, evt, setUnder);
		return !element_under_mouse_ || element_under_mouse_->DispatchMouseEvent(evt, type, clickCount);
	}

	void EventHandler::UpdateMouseEventTarget(UIElement* target, MouseEvent* evt, bool fireMouseOut)
	{
		UIElement* result = target;

		//处理capture 和 text

		element_under_mouse_ = target;

		if (fireMouseOut)
		{
			if (last_element_under_mouse_ != element_under_mouse_)
			{
				if (last_element_under_mouse_)
					last_element_under_mouse_->DispatchMouseEvent(evt, EVENT_MOUSE_OUT, 0, element_under_mouse_);
				if (element_under_mouse_)
					element_under_mouse_->DispatchMouseEvent(evt, EVENT_MOUSE_OVER, 0, last_element_under_mouse_);
			}
		}
		last_element_under_mouse_ = element_under_mouse_;

	}

}