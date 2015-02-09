#pragma once
#include "duiframework.h"

class AppViewController : public ui::UIViewController
{
public:
	AppViewController();
	~AppViewController();

	virtual void loadView() override;

	virtual void viewWillAppear() override;

	void buttonClicked();


protected:

};