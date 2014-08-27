#pragma once
#include "base/basictypes.h"
#include "event/event.h"

namespace ui
{
	class View;
	class KeyEvent : public Event
	{
	public:
		KeyEvent(EventType type, int key, View* v,
			LPARAM lparam);
		KeyEvent(EventType type, int key, View* v,
			uint8 count, uint8 flags);

		int GetKey() const;

		int GetKeyState() const;

		uint8 GetRepeatCounts() const;

		uint8 GetFlags() const;

		void SetSender(View* v);
		View* sender() const;
	private:
		int key_;
		int key_states_;
		uint8 count_;
		uint8 flags_;
		View* sender_;//消息来源
	};
}