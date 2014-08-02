#include "stdafx.h"
#include "control.h"

namespace ui
{


	Control::Control()
	{

	}

	Control::~Control()
	{

	}

	Control* Control::GetParent() const
	{
		return parent_;
	}

	int32 Control::GetChildCount() const
	{
		return controls_.size();
	}

	Control* Control::GetChild(int32 index)
	{
		if(index >= 0 && index < controls_.size())
			return controls_.at(index);
		return NULL;
	}

	void Control::AddChild(Control* control)
	{
		controls_.push_back(control);
	}

	bool Control::HasChild(Control* control)
	{
		for (Control* iter : controls_)
		{
			if (iter == control)
				return true;
		}
		return false;
	}

	View* Control::GetHostView() const
	{
		return view_;
	}

}