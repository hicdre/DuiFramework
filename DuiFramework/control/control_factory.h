#pragma once
#include "core/view.h"

namespace ui
{
	class ControlFactory
	{
	public:
		virtual ~ControlFactory() = 0;

		virtual View* Create(const wchar_t* class_name) = 0;
	};
}