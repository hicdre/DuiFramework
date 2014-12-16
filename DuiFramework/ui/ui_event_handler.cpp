#include "stdafx.h"
#include "ui_event_handler.h"
#include "ui_include.h"
#include "event/event_include.h"
#include <deque>

namespace ui
{


	UIEventHandler::UIEventHandler(UIWindow* window)
		: window_(window)
	{

	}

	UIEventHandler::~UIEventHandler()
	{

	}

	bool UIEventHandler::HandleMouseMoveEvent(MouseEvent* evt)
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

	bool UIEventHandler::DispatchMouseEvent(EventType type, UIElement* target, int clickCount, MouseEvent* evt, bool setUnder)
	{
		scoped_refptr<MouseEvent> mouseEvent(MouseEvent::Create(type, evt, clickCount, target));
		return !element_under_mouse_ || element_under_mouse_->DispatchEvent(mouseEvent.get());
	}

	void UIEventHandler::UpdateMouseEventTarget(UIElement* target, MouseEvent* evt)
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

	bool UIEventHandler::HandleMouseEvent(MouseEvent* evt)
	{
		//mouse move
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

		//mouse press
		if (evt->type() == EVENT_MOUSE_DOWN) {
			HandleMousePressEvent(evt);
		}
		else if (evt->type() == EVENT_MOUSE_UP) {
			HandleMouseReleaseEvent(evt);
		}

		UpdateCursor();


		return true;
	}

	void UIEventHandler::UpdateElementHoverState(MouseEvent* evt, HitTestResult* result)
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

	void UIEventHandler::UpdateCursor()
	{
		if (!element_under_mouse_)
			return;

		CursorId cursor = element_under_mouse_->cursor();
		if (cursor == last_cursor)
			return;

		window_->SetCursor(cursor);
		last_cursor = cursor;
	}

	bool UIEventHandler::HandleMousePressEvent(MouseEvent* evt)
	{
		element_pressed_ = element_under_mouse_;
		UpdateElementActiveFocusState();
		may_start_drag_ = true;
		DispatchMouseEvent(EVENT_MOUSE_DOWN, element_pressed_, evt->clickCount(), evt, true);
		return true;
	}

	void UIEventHandler::UpdateElementActiveFocusState()
	{
		if (!element_pressed_->IsHandleKeybordEvent()) {
			element_pressed_->SetActiveOrFocused(true);
			return;
		}
		UIElement* last_element_focused = element_focused_;
		element_focused_ = element_pressed_;
		element_pressed_->SetActiveOrFocused(true);//dispatch get focus
		last_element_focused->SetActiveOrFocused(false);//dispatch lose focus
	}

	bool UIEventHandler::HandleMouseReleaseEvent(MouseEvent* evt)
	{
		UIElement* last_element_pressed = element_pressed_;
		may_start_drag_ = false;
		DispatchMouseEvent(EVENT_MOUSE_UP, last_element_pressed, 0, evt, true);
		return true;
	}

}