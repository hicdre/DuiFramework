#include "stdafx.h"
#include "AppViewController.h"

using namespace ui;

AppViewController::AppViewController()
{

}

AppViewController::~AppViewController()
{

}


void AppViewController::buttonClicked()
{

}

void AppViewController::viewWillAppear()
{
	registerSelector("buttonClicked", std::bind(&AppViewController::buttonClicked, this));
}

void AppViewController::loadView()
{
	UIWindow* window = new UIWindow;
	window->InitWithBounds(ui::Rect(100, 100, 500, 400));
	window->SetBackgroundColor(ui::Color_Green);
	const wchar_t* text = L"Label 测试";
	//𪚥
	for (int i = 0; i < 9; ++i)
	{
		UILabel* label = new UILabel;
		int row = i / 3;
		int col = i % 3;
		label->InitWithBounds(Rect(20 + col * 120, 20 + row * 50, 100, 40));
		label->setText(text);
		label->SetBackgroundColor(Color_White);

		label->setTextAlignment((TextAlignment)col);
		label->setVerticalAlignment((VerticalAlignment)row);

		window->addSubView(label);
	}

	{
		UIButton* button = new UIButton;
		button->InitWithBounds(Rect(20, 200, 100, 40));
		button->setTitleforState(L"Button 测试", UIControlStateNormal);
		button->setTitleforState(L"Button Hovered", UIControlStateHovered);
		button->setTitleforState(L"Button Pressed", UIControlStatePressed);
		button->setTitleColorForState(Color_Black, UIControlStateNormal);
		button->SetBackgroundColor(Color_White);
		button->addTargetForControlEvents(this, "buttonClicked", UIControlEventClick);

		window->addSubView(button);
	}

	view_ = window;
	window->setRootViewController(this);
}

