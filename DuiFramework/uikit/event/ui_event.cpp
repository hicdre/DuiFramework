#include "stdafx.h"
#include "ui_event.h"
#include "uikit/event/ui_event_manager.h"

namespace ui
{
	UIEvent::UIEvent(UIEventType type)
		: type_(type)
		, timestamps_(0)
		, sub_type_(EVENT_SUB_NONE)
	{

	}

	UIMouse* UIEvent::mouse() const
	{
		return UIEventManager::Get()->mouse();
	}

}