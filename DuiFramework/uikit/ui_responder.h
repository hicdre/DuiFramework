#pragma once
#include "uikit/event/ui_event.h"
#include "uikit/event/ui_mouse.h"
#include "uikit/ui_object.h"

namespace ui
{
	class UIResponder : public UIObject
	{
	public:
		UIResponder();
		virtual ~UIResponder();

		virtual void Init();


		virtual UIResponder* NextResponder() const;

		virtual void mouseMove(UIMouse* mouse, UIEvent* event);
		virtual void mousePress(UIMouse* mouse, UIEvent* event);
		virtual void mouseRelease(UIMouse* mouse, UIEvent* event);

	};
}
