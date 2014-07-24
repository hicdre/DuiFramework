#pragma once
#include "core/view.h"
#include "event/event_listener.h"

namespace ui
{
	//表示一组view的组合体
	class ComplexView : public View
	{
	public:
		ComplexView();
		virtual ~ComplexView();

	protected:
		EventListener listener_;
	};
}