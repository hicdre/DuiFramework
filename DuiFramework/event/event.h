#pragma once
#include "base/basictypes.h"
#include "core/constants.h"
#include "event/event_forward.h"

namespace ui
{
	class Event : public RefCounted < Event >
	{
	public:
		virtual ~Event();

		void InitEvent(EventType type, bool canBubble, bool cancelable);

		EventType type() const { return type_; }
		void setType(EventType type) { type_ = type; }

		EventTarget* target() const { return target_.get(); }
		void setTarget(EventTarget* target);

		EventTarget* currentTarget() const;
		void setCurrentTarget(EventTarget* currentTarget);

		EventPhaseType eventPhase() const { return event_phase_; }
		void setEventPhase(EventPhaseType eventPhase) { event_phase_ = eventPhase; }

		bool bubbles() const { return can_bubble_; }
		bool cancelable() const { return cancelable_; }
		//DOMTimeStamp timeStamp() const { return m_createTime; }

		void stopPropagation() { is_propagation_stopped_ = true; }
		void stopImmediatePropagation() { is_immediate_propagation_stopped_ = true; }

		// These events are general classes of events.
		virtual bool IsUIEvent() const;
		virtual bool IsMouseEvent() const;
		virtual bool IsFocusEvent() const;
		virtual bool IsKeyboardEvent() const;
		virtual bool IsTouchEvent() const;
		virtual bool IsGestureEvent() const;
		virtual bool IsWheelEvent() const;
		virtual bool IsRelatedEvent() const;

		// Drag events are a subset of mouse events.
		virtual bool IsDragEvent() const;

		// These events lack a DOM interface.
		//virtual bool isClipboardEvent() const;
		//virtual bool isBeforeTextInsertedEvent() const;

		//virtual bool isBeforeUnloadEvent() const;

		bool propagationStopped() const { return is_propagation_stopped_ || is_immediate_propagation_stopped_; }
		bool immediatePropagationStopped() const { return is_immediate_propagation_stopped_; }

		bool defaultPrevented() const { return is_default_prevented_; }
		virtual void preventDefault()
		{
			if (cancelable_)
				is_default_prevented_ = true;
		}
		void setDefaultPrevented(bool defaultPrevented) { is_default_prevented_ = defaultPrevented; }

		bool defaultHandled() const { return is_default_handled_; }
		void setDefaultHandled() { is_default_handled_ = true; }

		bool cancelBubble() const { return is_cancel_bubble_; }
		void setCancelBubble(bool cancel) { is_cancel_bubble_ = cancel; }

		//Event* underlyingEvent() const { return m_underlyingEvent.get(); }
		//void setUnderlyingEvent(PassRefPtrWillBeRawPtr<Event>);

		EventPath& eventPath() { return *event_path_.get(); }
		EventPath& ensureEventPath();

		bool isBeingDispatched() const { return !!event_path_.get(); }

	protected:
		Event();
		Event(EventType type, bool canBubble, bool cancelable);
		//Event(EventType type, const EventInit&);

		virtual void ReceivedTarget();
		bool dispatched() const { return !!target_.get(); }

	private:
		EventType type_;

		bool can_bubble_;
		bool cancelable_;

		bool is_propagation_stopped_;
		bool is_immediate_propagation_stopped_;
		bool is_default_prevented_;
		bool is_default_handled_;
		bool is_cancel_bubble_;

		EventPhaseType event_phase_;
		scoped_refptr<EventTarget> current_target_;
		scoped_refptr<EventTarget> target_;
		//DOMTimeStamp m_createTime;
		scoped_ptr<EventPath> event_path_;
	};

}