#pragma once
#include "event/event.h"

namespace ui
{
	class Widget;
	class InputEvent : public Event
	{
	public:
		enum Modifiers {
			// modifiers for all events:
			ShiftKey = 1 << 0,
			ControlKey = 1 << 1,
			AltKey = 1 << 2,
			MetaKey = 1 << 3,

			// modifiers for keyboard events:
			IsKeyPad = 1 << 4,
			IsAutoRepeat = 1 << 5,

			// modifiers for mouse events:
			LeftButtonDown = 1 << 6,
			MiddleButtonDown = 1 << 7,
			RightButtonDown = 1 << 8,

			// Toggle modifiers for all events. Danger: these are not reflected
			// into WebCore, so round-tripping from WebInputEvent to a WebCore
			// event and back will not preserve these flags.
			CapsLockOn = 1 << 9,
			NumLockOn = 1 << 10,

			// Left/right modifiers for keyboard events.
			IsLeft = 1 << 11,
			IsRight = 1 << 12,

			// Last input event to be sent for the current vsync interval. If this
			// flag is set, the sender guarantees that no more input events will be
			// delivered until the next vsync and the receiver can schedule
			// rendering accordingly. If it isn't set, the receiver should not make
			// any assumptions about the delivery times of future input events
			// w.r.t. vsync.
			IsLastInputEventForCurrentVSync = 1 << 13,
		};

		virtual ~InputEvent() override;

		int modifiers() const { return modifiers_; }
		void setModifiers(int m) { modifiers_ = m; }

		double timestamps() const { return timestamps_; }
		void setTimestamps(double t) { timestamps_ = t; }
	protected:
		InputEvent();
		InputEvent(EventType type, bool canBubble, bool cancelable, int modifiers);

		double timestamps_; // Seconds since epoch.
		int modifiers_;
	};
}