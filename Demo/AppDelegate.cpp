#include "stdafx.h"
#include "AppDelegate.h"



void AppDelegate::OnApplicationInit()
{
	controller_.reset(new AppViewController);
	controller_->Init();
}

void AppDelegate::OnApplicationExit()
{

}
