#include "stdafx.h"
#include "event.h"

#include "event/event_include.h"

namespace ui
{
	Event::Event()
		: Event(EVENT_UNKNOWN, false, false)
	{

	}

	Event::Event(EventType type, bool canBubble, bool cancelable)
		: type_(type)
		, can_bubble_(canBubble)
		, cancelable_(cancelable)
		, is_propagation_stopped_(false)
		, is_immediate_propagation_stopped_(false)
		, is_default_prevented_(false)
		, is_default_handled_(false)
		, is_cancel_bubble_(false)
		, event_phase_(EVENT_NONE)
	{

	}

	Event::~Event()
	{

	}

	void Event::InitEvent(EventType type, bool canBubble, bool cancelable)
	{
		if (dispatched())
			return;

		type_ = type;
		can_bubble_ = canBubble;
		cancelable_ = cancelable;
	}

	void Event::setTarget(EventTarget* target)
	{
		if (target_.get() == target)
			return;

		target_ = target;
		if (target_)
			ReceivedTarget();
	}

	EventTarget* Event::currentTarget() const
	{
		if (!current_target_)
			return 0;
		return current_target_.get();
	}

	bool Event::IsUIEvent() const
	{
		return false;
	}

	bool Event::IsMouseEvent() const
	{
		return false;
	}

	bool Event::IsFocusEvent() const
	{
		return false;
	}

	bool Event::IsKeyboardEvent() const
	{
		return false;
	}

	bool Event::IsTouchEvent() const
	{
		return false;
	}

	bool Event::IsGestureEvent() const
	{
		return false;
	}

	bool Event::IsWheelEvent() const
	{
		return false;
	}

	bool Event::IsRelatedEvent() const
	{
		return false;
	}

	bool Event::IsDragEvent() const
	{
		return false;
	}
	
	void Event::ReceivedTarget()
	{

	}

	void Event::setCurrentTarget(EventTarget* currentTarget)
	{
		current_target_.reset(currentTarget);
	}

}