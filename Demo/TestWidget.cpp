#include "stdafx.h"
#include "TestWidget.h"

void TestWidget::OnInit()
{
	{
		std::wstring title_bg_path = LR"(E:\work\self\DuiFramwork\Demo\res\title_bg.png)";
		std::wstring global_btn_bkg_path = LR"(E:\work\self\DuiFramwork\Demo\res\global_btn_bkg.png)";
		std::wstring global_btn_icon_path = LR"(E:\work\self\DuiFramwork\Demo\res\global_btn_icon.png)";
		std::wstring button_close_path = LR"(E:\work\self\DuiFramwork\Demo\res\button_close.png)";
		ui::ImageStore* store = ui::ImageStore::Default();
		store->AddImageRecord("title", title_bg_path);
		store->AddImageRecord("close.normal", button_close_path, ui::Rect(0, 0, 10, 10));
		store->AddImageRecord("close.hover", button_close_path, ui::Rect(0, 10, 10, 10));
		store->AddImageRecord("close.pressed", button_close_path, ui::Rect(0, 20, 10, 10));
	}
	{
		ui::View* header = new ui::View;
		view()->Append(header);
		header->SetBounds(0, 0, 400, 50);
		header->set_background_image_id("title");
		{
			ui::Button* btn = new ui::Button;
			header->Append(btn);
			btn->SetBounds(360, 20, 10, 10);
			btn->SetStateImage(ui::Button::NORMAL, "close.normal");
			btn->SetStateImage(ui::Button::HOVERED, "close.hover");
			btn->SetStateImage(ui::Button::PRESSED, "close.pressed");
			listener_.Listen(btn, ui::EVENT_BUTTON_CLICKED, [this](ui::View* target, ui::Event* evt)
			{
				view()->Close();
				ui::App::Get()->Quit();
			});
		}
	}

	view()->set_background_color(ui::ColorSetRGB(255, 255, 255));
	view()->CenterWidget();
}

ui::Rect TestWidget::GetInitialRect()
{
	return ui::Rect(0, 0, 400, 300);
}
