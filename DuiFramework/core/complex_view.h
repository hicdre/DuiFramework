#pragma once
#include "core/view.h"

namespace ui
{
	//表示一组view的组合体
	class ComplexView : public View
	{
	public:
		ComplexView();
		virtual ~ComplexView();

		//void Listen(View* subview, EventType type, EventAction action);
		//void Listen(EventType type, EventAction action);
	protected:
		//EventListener listener_;
	};
}