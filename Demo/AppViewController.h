#pragma once
#include "AppView.h"

class AppViewController : public ui::UIViewController
{
public:
	AppViewController();
	~AppViewController();

	virtual ui::UIView* view() override;

	virtual void viewWillAppear() override;

	void buttonClicked();


protected:

};