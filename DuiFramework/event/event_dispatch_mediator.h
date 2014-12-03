#pragma once
#include "event/event_forward.h"

namespace ui
{
	class EventDispatchMediator : public RefCounted < EventDispatchMediator >
	{
	public:
		static scoped_refptr<EventDispatchMediator> Create(Event* event);
		virtual ~EventDispatchMediator() { };
		//virtual void trace(Visitor*);
		virtual bool DispatchEvent(EventDispatcher*) const;
		Event* event() const { return event_.get(); };

	protected:
		explicit EventDispatchMediator(Event* event);
		EventDispatchMediator() { };
		void setEvent(Event* event) { event_.reset(event); };

	private:
		scoped_refptr<Event> event_;
	};
}