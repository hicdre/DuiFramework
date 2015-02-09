#include "stdafx.h"
#include "AppDelegate.h"



void AppDelegate::OnApplicationInit()
{
	controller_.reset(new AppViewController);
	controller_->Init();

	controller_->view()->window()->AddToScreen();
}

void AppDelegate::OnApplicationExit()
{

}
