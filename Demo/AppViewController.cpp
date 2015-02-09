#include "stdafx.h"
#include "AppViewController.h"

AppViewController::AppViewController()
{

}

AppViewController::~AppViewController()
{

}

ui::UIView* AppViewController::view()
{
	if (!view_)
	{
		view_ = new AppView;
		view_->InitWithBounds(ui::Rect(100,100, 500,400));
	}
	return view_;
}

void AppViewController::buttonClicked()
{

}

void AppViewController::viewWillAppear()
{

}

