#include "stdafx.h"
#include "ui_object.h"

namespace ui
{

	UIObject::~UIObject()
	{

	}

	void UIObject::registerSelector(const Selector& s, FunctionType func)
	{
		function_map_[s] = func;
	}

	void UIObject::performSelector(const Selector& s)
	{
		auto iter = function_map_.find(s);
		if (iter == function_map_.end())
			return;
		iter->second();
	}

}