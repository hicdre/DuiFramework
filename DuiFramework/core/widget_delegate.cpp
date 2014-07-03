#include "stdafx.h"
#include "widget_delegate.h"

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