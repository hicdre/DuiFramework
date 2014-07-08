#pragma once
#include "duiframework.h"

class TestWidget
	: public ui::WidgetDelegate
{
public:
	virtual void OnInit() override;
	virtual ui::Rect GetInitialRect() override;

private:
	ui::EventListener listener_;

};