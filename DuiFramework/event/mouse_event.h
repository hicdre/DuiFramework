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

		void SetSender(View* v);
		View* sender() const;

	protected:
		Point pt_in_widget_; 
		int flags_{ 0 };//mouse key flags;
		View* sender_;//消息来源
	};

}