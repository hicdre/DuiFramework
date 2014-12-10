#include "stdafx.h"
#include "event_handler.h"
#include "event_include.h"
#include "dom/ui_window.h"
#include "dom/ui_styles.h"
#include <deque>

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
		if (!window_->Hittest(&hitResult, evt->clientLocation())) {
			assert(0);
			return false;
		}

		UpdateMouseEventTarget(hitResult.element(), evt);

		UpdateElementHoverState(evt, &hitResult);

		DispatchMouseEvent(EVENT_MOUSE_MOVE, hitResult.element(), 0, evt, true);

		return false;
	}

	bool EventHandler::DispatchMouseEvent(EventType type, UIElement* target, int clickCount, MouseEvent* evt, bool setUnder)
	{
		scoped_refptr<MouseEvent> mouseEvent(MouseEvent::Create(EVENT_MOUSE_MOVE, evt, clickCount, target));
		return !element_under_mouse_ || element_under_mouse_->DispatchEvent(mouseEvent.get());
	}

	void EventHandler::UpdateMouseEventTarget(UIElement* target, MouseEvent* evt)
	{
		UIElement* result = target;

		//¥¶¿Ìcapture ∫Õ text
		last_element_under_mouse_ = element_under_mouse_;
		element_under_mouse_ = target;

		if (last_element_under_mouse_ != element_under_mouse_)
		{
			if (last_element_under_mouse_) {
				scoped_refptr<MouseEvent> mouseEvent(MouseEvent::Create(EVENT_MOUSE_OUT, evt, 0, element_under_mouse_));
				last_element_under_mouse_->DispatchEvent(mouseEvent.get());
			}
				
			if (element_under_mouse_) {
				scoped_refptr<MouseEvent> mouseEvent(MouseEvent::Create(EVENT_MOUSE_OVER, evt, 0, last_element_under_mouse_));
				element_under_mouse_->DispatchEvent(mouseEvent.get());
			}
			
		}
	}

	bool EventHandler::HandleMouseEvent(MouseEvent* evt)
	{
		Point last_position; 
		if (last_mouse_event_) {
			last_position = last_mouse_event_->clientLocation();
		}
		if (last_position != evt->clientLocation()) {
			int dx = evt->clientLocation().x() - last_position.x();
			int dy = evt->clientLocation().y() - last_position.y();
			evt->setMovementDelta(dx, dy);

			HandleMouseMoveEvent(evt);

			last_mouse_event_ = evt;
		}


		UpdateCursor();


		return true;
	}

	void EventHandler::UpdateElementHoverState(MouseEvent* evt, HitTestResult* result)
	{
		if (last_element_under_mouse_ == element_under_mouse_)
			return;

		scoped_ptr<EventPath> mouseLeavePath(new EventPath);
		scoped_ptr<EventPath> mouseEnterPath(new EventPath);

		{
			UIElement* e = last_element_under_mouse_;
			while (e) {
				mouseLeavePath->push_back(e);
				e = e->parent().get();
			}
		}

		{
			UIElement* e = element_under_mouse_;
			while (e) {
				mouseEnterPath->push_back(e);
				e = e->parent().get();
			}
		}

		while (mouseLeavePath->back() == mouseEnterPath->back() && !mouseLeavePath->isEmpty()) {
			mouseLeavePath->pop_back();
			mouseEnterPath->pop_back();
		}

		mouseEnterPath->reverse();

		{
			scoped_refptr<MouseEvent> mouseEvent(MouseEvent::Create(EVENT_MOUSE_LEAVE, evt, 0, NULL));
			size_t size = mouseLeavePath->size();
			for (size_t i = 0; i < size; ++i) {
				UIElement* elem = mouseLeavePath->at(i);
				elem->SetHovered(false);
				scoped_ptr<EventPath> path(new EventPath);
				path->push_back(elem);
				elem->DispatchEvent(mouseEvent.get(), path.release());
			}
		}

		{
			scoped_refptr<MouseEvent> mouseEvent(MouseEvent::Create(EVENT_MOUSE_ENTER, evt, 0, NULL));
			size_t size = mouseEnterPath->size();
			for (size_t i = 0; i < size; ++i) {
				UIElement* elem = mouseEnterPath->at(i);
				elem->SetHovered(true);
				scoped_ptr<EventPath> path(new EventPath);
				path->push_back(elem);
				elem->DispatchEvent(mouseEvent.get(), path.release());
			}
		}
	}

	void EventHandler::UpdateCursor()
	{
		if (!element_under_mouse_)
			return;

		CursorId cursor = element_under_mouse_->cursor();
		if (cursor == last_cursor)
			return;

		window_->SetCursor(cursor);
		last_cursor = cursor;
	}

}