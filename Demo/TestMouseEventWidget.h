#pragma once
#include "core/widget_delegate.h"
#include "core/event.h"
#include "core/label.h"

class TestMouseEventWidget 
	: public ui::WidgetDelegate
	, public ui::EventListener
{
public:
	virtual void OnInit() override;
	virtual ui::Rect GetInitialRect() override;

	virtual void OnMouseMove(ui::View* v, ui::MouseEvent& event) override;

private:
	ui::Label* labe_;
};
