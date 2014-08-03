#include "stdafx.h"
#include "event_dispatcher.h"

namespace ui
{
	EventDispatcher::~EventDispatcher()
	{
		for (auto iter : dispatch_map_)
		{
			for (auto iter2 : iter.second)
			{
				EventHandlerList* handler_list = iter2.second;
				for (EventHandler* handler : *handler_list)
				{
					delete handler;
				}
				delete handler_list;
			}
		}
	}

	EventDispatcher* EventDispatcher::FindByWindow(Window* window)
	{

	}

	void EventDispatcher::ReleaseAll()
	{

	}

	void EventDispatcher::RemoveByView(View* v)
	{
		auto iter = dispatch_map_.find(v);
		if (iter == dispatch_map_.end())
			return;

		for (auto iter2 : iter->second)
		{
			EventHandlerList* handler_list = iter2.second;
			for (EventHandler* handler : *handler_list)
			{
				delete handler;
			}
			delete handler_list;
		}

		dispatch_map_.erase(iter);
	}

	

}