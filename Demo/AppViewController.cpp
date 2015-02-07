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
		view_->InitWithBounds(ui::Rect(100,100, 300,200));
	}
	return view_;
}

