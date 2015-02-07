#pragma once
#include "duiframework.h"
#include "AppViewController.h"

class AppDelegate : public ui::UIApplicationDelegate
{
public:
	virtual void OnApplicationInit() override;
	virtual void OnApplicationExit() override;

private:
	scoped_ptr<AppViewController> controller_;
};