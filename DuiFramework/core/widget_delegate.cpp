#include "stdafx.h"
#include "widget_delegate.h"

#include "core/widget_view.h"

namespace ui
{

	WidgetDelegate::WidgetDelegate()
	{

	}

	WidgetDelegate::~WidgetDelegate()
	{

	}

	void WidgetDelegate::Init(WidgetView* view)
	{
		widget_view_ = view;

		OnInit();
	}

}