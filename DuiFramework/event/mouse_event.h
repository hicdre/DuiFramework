#pragma once
#include "event/event.h"

namespace ui
{
	class MouseEvent : public Event
	{
	public:
		int flags() const;

		bool IsOnlyLeftMouseButton() const;

		bool IsLeftMouseButton() const;

		bool IsOnlyMiddleMouseButton() const;

		bool IsMiddleMouseButton() const;

		bool IsOnlyRightMouseButton() const;

		bool IsRightMouseButton() const;

		bool IsAnyButton() const;

		Point GetPosition() const;

		static void DispatchTo(MouseEvent* event, View* v);
	protected:
		friend class EventDispatcher;
		MouseEvent(EventType type, const Point& pt, View* owner);
		Point pt_; //在sender_坐标系中
		int flags_{ 0 };//mouse flags;
	};
}