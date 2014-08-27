#pragma once
#include "base/basictypes.h"
#include <vector>

namespace ui
{
	class View;
	class Control
	{
	public:
		Control();
		virtual ~Control();

		typedef std::vector<Control*> Controls;

		Control* GetParent() const;
		int32 GetChildCount() const;
		Control* GetChild(int32 index);
		void AddChild(Control* control);
		bool HasChild(Control* control);


		View* GetHostView() const;
	protected:
		Control* parent_;
		Controls controls_;
		View* view_;
		//EventListener listener_;

	};
}