#include "stdafx.h"
#include "event_listener.h"
#include "event/event_listen_manager.h"

namespace ui
{
	EventListener::EventListener()
	{
		EventListenManager::Default()->AddListener(this);
	}

	EventListener::~EventListener()
	{
		EventListenManager::Default()->RemoveListener(this);
	}

	EventListener& EventListener::Listen(View* from, EventType type, EventAction action)
	{
		event_map_[from][type] = action;
		return *this;
	}

	void EventListener::UnListen(View* from, EventType type)
	{
		event_map_[from].erase(type);
	}

	void EventListener::UnListen(View* from)
	{
		event_map_.erase(from);
	}

	void EventListener::DispatchEvent(View* v, Event* e)
	{
		if (!event_map_.count(v))
			return;

		EventType type = e->type();
		if (!event_map_[v].count(type))
			return;

		event_map_[v][type](v, e);
	}
}