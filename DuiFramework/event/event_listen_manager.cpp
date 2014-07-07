#include "stdafx.h"
#include "event_dispatcher.h"

#include "core/app.h"

namespace ui
{
	EventListenManager::EventListenManager()
	{

	}

	EventListenManager::~EventListenManager()
	{

	}

	void EventListenManager::AddListener(EventListener* listener)
	{
		listener_list_.push_back(listener);
	}

	void EventListenManager::RemoveListener(EventListener* listener)
	{
		for (auto iter = listener_list_.begin(); iter != listener_list_.end(); iter++)
		{
			if (*iter == listener)
			{
				listener_list_.erase(iter);
				return;
			}
		}
	}


	EventListenManager* EventListenManager::Default()
	{
		return App::Get()->DefaultEventListenManager();
	}

	void EventListenManager::DispatchEvent(View* v, Event* e)
	{
		for (EventListener* l : listener_list_)
		{
			l->DispatchEvent(v, e);
		}
	}

	void EventListenManager::RemoveView(View* v)
	{
		for (EventListener* l : listener_list_)
		{
			l->UnListen(v);
		}
	}
}