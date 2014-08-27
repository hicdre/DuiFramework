#pragma once

namespace ui
{
	class View;
	class FocusManager
	{
	public:
		virtual ~FocusManager() {}

		virtual void SetFocus(View* v) = 0;
	};
}