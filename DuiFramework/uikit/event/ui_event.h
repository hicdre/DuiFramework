#pragma once
#include "foundation/foundation.h"
#include "uikit/common/ui_constants.h"
#include "uikit/event/ui_mouse.h"

namespace ui
{
	class UIEvent
	{
	public:
		UIEvent(UIEventType type);
		virtual ~UIEvent() {}

		UIEventType type() const { return type_; }
		void setType(UIEventType type) { type_ = type; }

		UIEventSubType subType() const { return sub_type_; }
		void setSubType(UIEventSubType type) { sub_type_ = type; }

		void setTimestamps(int64 val) { timestamps_ = val; }
		int64 timestamps() const { return timestamps_; }

		UIMouse* mouse() const;
		
	private:
		UIEventType type_;
		UIEventSubType sub_type_;
		int64 timestamps_;
	};
}