#pragma once
#include "foundation/foundation.h"
#include "uikit/event/ui_event.h"
#include "uikit/event/ui_mouse.h"
#include <unordered_set>
namespace ui
{
	class UIWindow;
	class UIControl;
	class UIEventManager
	{
	public:
		UIEventManager();
		~UIEventManager();

		static UIEventManager* Get();

		void HanleMessage(UIWindow* window, UINT message,
			WPARAM w_param,
			LPARAM l_param);

		UIMouse* mouse();
	private:
		struct UIMouseEventRecord
		{
			UIEvent* event;
			UIMouse* mouse;
			UIMouseEventRecord* next;
			UIMouseEventRecord* prev;
		};
		void HandleMouseEvent(UIWindow* window, UINT message,
			WPARAM w_param,
			LPARAM l_param);

		UIEvent* BuildMouseEvent(UIWindow* window,
			UINT message,
			WPARAM w_param,
			LPARAM l_param);

		Point last_mouse_position_;
		scoped_ptr<UIMouse> mouse_;

		std::unordered_set<UIControl*> tracking_controls_;
	};
}