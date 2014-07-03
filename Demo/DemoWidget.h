#pragma once
#include "core/widget_delegate.h"

class DemoWidget : public ui::WidgetDelegate
{
public:
	virtual void OnInit() override;
	virtual ui::Rect GetInitialRect() override;
};
