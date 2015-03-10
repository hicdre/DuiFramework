#include "stdafx.h"
#include "AppViewController.h"

using namespace ui;

AppViewController::AppViewController()
{

}

AppViewController::~AppViewController()
{

}



void AppViewController::viewWillAppear()
{
	registerSelector("closeClicked", std::bind(&AppViewController::closeClicked, this));
	registerSelector("textAlignmentButtonClicked", std::bind(&AppViewController::textAlignmentButtonClicked, this, std::placeholders::_1));
	registerSelector("verticalAlignmentButtonClicked", std::bind(&AppViewController::verticalAlignmentButtonClicked, this, std::placeholders::_1));
}

void AppViewController::loadView()
{
	UIWindow* window = new UIWindow;
	window->InitWithBounds(ui::Rect(100, 100, 500, 400));
	window->SetBackgroundColor(ui::Color_Green);
	const wchar_t* text = L"Label 测试";
	//𪚥
	{
		UIButton* button = new UIButton;
		button->InitWithBounds(Rect(480, 0, 20, 20));
		button->setTitleforState(L"X", UIControlStateNormal);
		button->setTitleColorForState(Color_Black, UIControlStateNormal);
		button->setTitleColorForState(Color_Red, UIControlStateHovered);
		button->addTargetForControlEvents(this, "closeClicked", UIControlEventClick);

		window->addSubView(button);
	}

	{
		UILabel* label = new UILabel;
		label->InitWithBounds(Rect(20, 20, 80, 20));
		label->setText(L"水平对齐：");
		label->setTextAlignment(TextAlignmentRight);
		window->addSubView(label);
	}
	{
		UIButton* button = new UIButton;
		button->InitWithBounds(Rect(110, 20, 80, 20));
		button->setTitleforState(L"Left", UIControlStateNormal);
		button->setTitleColorForState(Color_Black, UIControlStateNormal);
		button->setTitleColorForState(Color_Green, UIControlStateHovered);
		button->SetBackgroundColor(Color_White);
		button->addTargetForControlEvents(this, "textAlignmentButtonClicked", UIControlEventClick);

		window->addSubView(button);

		textAlignmentButtons_[TextAlignmentLeft] = button;
	}

	{
		UIButton* button = new UIButton;
		button->InitWithBounds(Rect(200, 20, 80, 20));
		button->setTitleforState(L"Center", UIControlStateNormal);
		button->setTitleColorForState(Color_Black, UIControlStateNormal);
		button->setTitleColorForState(Color_Green, UIControlStateHovered);
		button->SetBackgroundColor(Color_White);
		button->addTargetForControlEvents(this, "textAlignmentButtonClicked", UIControlEventClick);

		window->addSubView(button);
		textAlignmentButtons_[TextAlignmentCenter] = button;
	}

	{
		UIButton* button = new UIButton;
		button->InitWithBounds(Rect(290, 20, 80, 20));
		button->setTitleforState(L"Right", UIControlStateNormal);
		button->setTitleColorForState(Color_Black, UIControlStateNormal);
		button->setTitleColorForState(Color_Green, UIControlStateHovered);
		button->SetBackgroundColor(Color_White);
		button->addTargetForControlEvents(this, "textAlignmentButtonClicked", UIControlEventClick);

		window->addSubView(button);
		textAlignmentButtons_[TextAlignmentRight] = button;
	}

	{
		UILabel* label = new UILabel;
		label->InitWithBounds(Rect(20, 50, 80, 20));
		label->setText(L"垂直对齐：");
		label->setTextAlignment(TextAlignmentRight);
		window->addSubView(label);
	}
	{
		UIButton* button = new UIButton;
		button->InitWithBounds(Rect(110, 50, 80, 20));
		button->setTitleforState(L"Top", UIControlStateNormal);
		button->setTitleColorForState(Color_Black, UIControlStateNormal);
		button->setTitleColorForState(Color_Green, UIControlStateHovered);
		button->SetBackgroundColor(Color_White);
		button->addTargetForControlEvents(this, "verticalAlignmentButtonClicked", UIControlEventClick);

		window->addSubView(button);
		verticalAlignmentButtons_[VerticalAlignmentTop] = button;
	}

	{
		UIButton* button = new UIButton;
		button->InitWithBounds(Rect(200, 50, 80, 20));
		button->setTitleforState(L"Middle", UIControlStateNormal);
		button->setTitleColorForState(Color_Black, UIControlStateNormal);
		button->setTitleColorForState(Color_Green, UIControlStateHovered);
		button->SetBackgroundColor(Color_White);
		button->addTargetForControlEvents(this, "verticalAlignmentButtonClicked", UIControlEventClick);

		window->addSubView(button);
		verticalAlignmentButtons_[VerticalAlignmentMiddle] = button;
	}

	{
		UIButton* button = new UIButton;
		button->InitWithBounds(Rect(290, 50, 80, 20));
		button->setTitleforState(L"Bottom", UIControlStateNormal);
		button->setTitleColorForState(Color_Black, UIControlStateNormal);
		button->setTitleColorForState(Color_Green, UIControlStateHovered);
		button->SetBackgroundColor(Color_White);
		button->addTargetForControlEvents(this, "verticalAlignmentButtonClicked", UIControlEventClick);

		window->addSubView(button);
		verticalAlignmentButtons_[VerticalAlignmentBottom] = button;
	}

	{
		UILabel* label = new UILabel;
		label->InitWithBounds(Rect(150, 150, 200, 150));
		label->setText(L"Label测试");
		label->SetBackgroundColor(Color_White);
		label->setTextAlignment(TextAlignmentLeft);
		window->addSubView(label);

		textLabel_ = label;
	}

	view_ = window;
	window->setRootViewController(this);
}

ui::TextAlignment AppViewController::textAlignmentFromButton(UIButton* btn)
{
	for (int i = 0; i < textAlignmentButtonCount; ++i)
	{
		if (textAlignmentButtons_[i] == btn)
			return (TextAlignment)i;
	}
	return TextAlignmentLeft;
}

ui::VerticalAlignment AppViewController::verticalAlignmentFromButton(UIButton* btn)
{
	for (int i = 0; i < verticalAlignmentButtonCount; ++i)
	{
		if (verticalAlignmentButtons_[i] == btn)
			return (VerticalAlignment)i;
	}
	return VerticalAlignmentMiddle;
}

void AppViewController::textAlignmentButtonClicked(UIObject* target)
{
	UIButton* button = static_cast<UIButton*>(target);
	textLabel_->setTextAlignment(textAlignmentFromButton(button));
}

void AppViewController::verticalAlignmentButtonClicked(ui::UIObject* target)
{
	UIButton* button = static_cast<UIButton*>(target);
	textLabel_->setVerticalAlignment(verticalAlignmentFromButton(button));
}

void AppViewController::closeClicked()
{
	UIApplication::Get()->Quit();
}

