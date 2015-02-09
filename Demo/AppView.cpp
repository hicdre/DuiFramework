#include "stdafx.h"
#include "AppView.h"

using namespace ui;
void AppView::InitSubViews()
{
	UIWindow::InitSubViews();
	SetBackgroundColor(ui::Color_Green);
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

		addSubView(label);
	}

	{
		UIButton* button = new UIButton;
		button->InitWithBounds(Rect(20, 200, 100 , 40));
		button->setTitleforState(L"Button 测试", UIControlStateNormal);

		addSubView(button);
		
	}
}
