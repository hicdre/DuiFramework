#include "stdafx.h"
#include "mouse_event.h"
#include "event_include.h"
#include "ui/ui_include.h"
#include <cassert>

namespace ui
{
	MouseEvent::MouseEvent()
		: m_button(ButtonNone)
		, m_clickCount(0)
		, m_needCalcLocation(true)
	{

	}

	MouseEvent::MouseEvent(EventType eventType, 
		bool canBubble, bool cancelable,
		int modifiers,
		const Point& screenLocation,
		const Point& clientLocation,
		const Point& movementDelta,
		Button button,
		int detail, UIElement* relatedTarget)
		: InputEvent(eventType, canBubble, cancelable, modifiers)
		, m_screenLocation(screenLocation)
		, m_clientLocation(clientLocation)
		, m_movementDelta(movementDelta)
		, m_clickCount(detail)
		, related_target_(relatedTarget)
		, m_needCalcLocation(true)
		, m_button(button)
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


	MouseEvent* MouseEvent::Create()
	{
		return new MouseEvent;
	}


	MouseEvent* MouseEvent::Create(EventType eventType, MouseEvent* other, int detail, UIElement* relatedTarget)
	{
		assert(other->type() == EVENT_MOUSE_MOVE || other->button() != ButtonNone);

		bool isMouseEnterOrLeave = eventType == EVENT_MOUSE_ENTER || eventType == EVENT_MOUSE_LEAVE;
		bool isCancelable = !isMouseEnterOrLeave;
		bool isBubbling = !isMouseEnterOrLeave;

		return new MouseEvent(eventType, isBubbling, isCancelable, other->modifiers(),
			other->screenLocation(), other->clientLocation(), other->movementDelta(),
			other->button(), detail, relatedTarget);
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

	void MouseEvent::AdjustForTarget()
	{
		m_needCalcLocation = true;
	}


}