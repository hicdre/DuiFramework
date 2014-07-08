#pragma once
#include "event/event.h"

namespace ui
{
	class View;
	class MouseEvent : public Event
	{
	public:
		MouseEvent(EventType type, const Point& pt_in_widget, View* sender);

		Point GetPosition(View* v) const;

		int GetMouseKeyFlags() const;

		bool HasMouseDown() const;

	protected:
		Point pt_in_widget_; 
		int flags_{ 0 };//mouse key flags;
		
	};

	class MouseDownEvent : public MouseEvent
	{
	public:
		MouseDownEvent(EventType type, const Point& pt_in_widget, View* sender, int buttons);

		int GetMouseButtons() const;
	protected:
		int buttons_;//mouse buttons;
	};

	typedef MouseDownEvent MouseUpEvent;
	typedef MouseDownEvent MouseDbClickEvent;

}