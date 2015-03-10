#pragma once
#include "duiframework.h"

class AppViewController : public ui::UIViewController
{
public:
	AppViewController();
	~AppViewController();

	virtual void loadView() override;

	virtual void viewWillAppear() override;

	void textAlignmentButtonClicked(ui::UIObject* target);
	void verticalAlignmentButtonClicked(ui::UIObject* target);
	void closeClicked();
protected:
	ui::TextAlignment textAlignmentFromButton(ui::UIButton* btn);
	ui::VerticalAlignment verticalAlignmentFromButton(ui::UIButton* btn);
	static const int textAlignmentButtonCount = 3;
	static const int verticalAlignmentButtonCount = 3;
	ui::UIButton* textAlignmentButtons_[textAlignmentButtonCount];
	ui::UIButton* verticalAlignmentButtons_[verticalAlignmentButtonCount];
	ui::UILabel* textLabel_;
};