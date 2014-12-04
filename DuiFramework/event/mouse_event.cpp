#include "stdafx.h"
#include "mouse_event.h"
#include "event_include.h"
#include "dom/ui_include.h"
#include <cassert>

namespace ui
{
	MouseEvent::MouseEvent()
		: m_button(ButtonNone)
		, m_clickCount(0)
	{

	}

	int MouseEvent::x()
	{
		CalcLocation();
		return m_point.x();
	}

	int MouseEvent::y()
	{
		CalcLocation();
		return m_point.y();
	}

	bool MouseEvent::IsMouseEvent() const
	{
		return true;
	}

	bool MouseEvent::IsDragEvent() const
	{
		return false;
	}

	MouseEvent* MouseEvent::Create(EventType eventType, MouseEvent* other, int detail, UIElement* relatedTarget)
	{
		assert(other->type() == EVENT_MOUSE_MOVE || other->button() != ButtonNone);

		bool isMouseEnterOrLeave = eventType == EVENT_MOUSE_ENTER || eventType == EVENT_MOUSE_LEAVE;
		bool isCancelable = !isMouseEnterOrLeave;
		bool isBubbling = !isMouseEnterOrLeave;

		MouseEvent* new_event = new MouseEvent;
		new_event->m_screenLocation = other->m_screenLocation;
		new_event->m_clientLocation = other->m_clientLocation;
		new_event->m_movementDelta = other->m_movementDelta;
		new_event->m_point = other->m_point;
		new_event->m_button = other->m_button;
		new_event->m_clickCount = other->m_clickCount;
		new_event->InitEvent(eventType, isBubbling, isCancelable);
		//test
		return new_event;
	}

	void MouseEvent::CalcLocation()
	{
		if (!m_needCalcLocation)
			return;
		UIElement* elem = dynamic_cast<UIElement*>(currentTarget());
		if (elem) {
			m_point = UIElement::ConvertPointToElement(elem, m_clientLocation);
		}
		m_needCalcLocation = false;
	}


}