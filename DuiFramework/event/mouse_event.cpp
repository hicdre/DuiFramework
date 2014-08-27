#include "stdafx.h"
#include "mouse_event.h"

#include "core/view.h"

namespace ui
{


	MouseEvent::MouseEvent(EventType type, const Point& pt_in_widget, View* sender)
		: Event(type)
		, pt_in_widget_(pt_in_widget)
		, flags_(GetMouseKeyFlags())
		, sender_(sender)
	{

	}

	Point MouseEvent::GetPosition(View* v) const
	{
		return v->ConvertPointFromWidget(pt_in_widget_);
	}

	int MouseEvent::GetMouseKeyFlags() const
	{
		return flags_;
	}

	bool MouseEvent::HasMouseDown() const
	{
		return (flags_ & (MOUSE_LEFT | MOUSE_RIGHT | MOUSE_MIDDLE)) != 0;
	}

	void MouseEvent::SetSender(View* v)
	{
		sender_ = v;
	}

	View* MouseEvent::sender() const
	{
		return sender_;
	}


}