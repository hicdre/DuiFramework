#include "stdafx.h"
#include "mouse_event.h"

#include "core/view.h"

namespace ui
{

	MouseEvent::MouseEvent(EventType type, const Point& pt, View* owner)
		: Event(type, owner)
		, pt_(pt)
		, flags_(GetKeyFlags())
	{

	}

	int MouseEvent::flags() const
	{
		return flags_;
	}

	bool MouseEvent::IsAnyButton() const
	{
		return (flags() & (MOUSE_LEFT | MOUSE_MIDDLE |
			MOUSE_RIGHT)) != 0;
	}

	bool MouseEvent::IsRightMouseButton() const
	{
		return (flags() & MOUSE_RIGHT) != 0;
	}

	bool MouseEvent::IsOnlyRightMouseButton() const
	{
		return (flags() & MOUSE_RIGHT) &&
			!(flags() & (MOUSE_LEFT | MOUSE_MIDDLE));
	}

	bool MouseEvent::IsMiddleMouseButton() const
	{
		return (flags() & MOUSE_MIDDLE) != 0;
	}

	bool MouseEvent::IsOnlyMiddleMouseButton() const
	{
		return (flags() & MOUSE_MIDDLE) &&
			!(flags() & (MOUSE_LEFT | MOUSE_RIGHT));
	}

	bool MouseEvent::IsLeftMouseButton() const
	{
		return (flags() & MOUSE_LEFT) != 0;
	}

	bool MouseEvent::IsOnlyLeftMouseButton() const
	{
		return (flags() & MOUSE_LEFT) &&
			!(flags() & (MOUSE_MIDDLE | MOUSE_RIGHT));
	}

	Point MouseEvent::GetPosition() const
	{
		return pt_;
	}

	void MouseEvent::DispatchTo(MouseEvent* event, View* v)
	{
		if (event->sender() == v)
			return;

		if (event->sender() == NULL)
		{
			event->pt_ = v->ConvertPointFromWidget(event->pt_);
		}
		else
		{
			View::ConvertPointToTarget(event->sender(), v, &(event->pt_));
		}
		event->SetSender(v);
		v->HandleEvent(event);
	}

}