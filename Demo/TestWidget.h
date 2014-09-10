#pragma once
#include "duiframework.h"

class TestWindowConstructor
{
public:
	ui::Window* Create();



private:
	void OnCreate();
	void OnInit();
	ui::Window* view() const;
	ui::Window* window_;
	ui::TextView* label_;
	ui::Point anchor_;
};