#include "stdafx.h"
#include "AppView.h"

using namespace ui;
void AppView::InitWithBounds(const ui::Rect& rect)
{
	UIWindow::InitWithBounds(rect);
	SetBackgroundColor(ui::Color_Red);
	{
		UILabel* label = new UILabel;
		label->InitWithBounds(Rect(20,50,100,12));
		label->SetBackgroundColor(Color_White);
		label->setTextColor(Color_Black);
		label->setText(L"H我 𪚥天天");


		Append(label);
	}

	
}
