#include "stdafx.h"
#include "TestWidget.h"


ui::Window* TestWindowConstructor::Create()
{
	ui::Widget* widget = ui::Widget::Create();
	window_ = new ui::Window;
	OnCreate();
	window_->AttachWidget(widget);
	OnInit();
	return window_;
}

void TestWindowConstructor::OnCreate()
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
			ui::TextButton* btn = new ui::TextButton();
			header->Append(btn);
			btn->SetBounds(360, 20, 10, 10);
			btn->SetStateImage(ui::Button::NORMAL, "close.normal");
			btn->SetStateImage(ui::Button::HOVERED, "close.hover");
			btn->SetStateImage(ui::Button::PRESSED, "close.pressed");
			btn->listener().Listen(ui::EVENT_BUTTON_CLICKED, [this](ui::Event* evt)
			{
				view()->Close();
				ui::App::Get()->Quit();
			});
		}
	}
	{
		label_ = new ui::TextView;
		label_->SetBounds(0, 50, 100, 20);
		label_->SetFont(L"Consolas", 12);
		label_->SetHorizontalAlignment(ui::ALIGN_CENTER);
		label_->SetTextColor(ui::ColorSetRGB(0, 0, 0));
		view()->Append(label_);
	}

	{
#if 0
		ui::BoxLayout* box = new ui::BoxLayout(true);
		box->SetBounds(0, 70, 100, 200);
		box->set_background_color(ui::ColorSetRGB(255, 0, 0));
		view()->Append(box);
		{
			ui::Container* c = new ui::Container();
			c->SetSize(ui::Size(100, 20));
			c->set_background_color(ui::ColorSetRGB(0, 125, 0));
			box->Append(c);
		}
		{
			ui::Container* c = new ui::Container();
			c->SetSize(ui::Size(80, 20));
			c->set_background_color(ui::ColorSetRGB(0, 0, 125));
			box->Append(c);
		}
#endif
	}
	view()->set_background_color(ui::ColorSetRGB(255, 255, 255));
	view()->SetFocusable(true);
	view()->listener().Listen(ui::EVENT_CHAR, [this](ui::Event* evt)
	{
		ui::KeyEvent* key_event = static_cast<ui::KeyEvent*>(evt);
		label_->SetText(label_->text() + std::wstring(1, (wchar_t)key_event->GetKey()));
	}).Listen(ui::EVENT_DRAG_BEGIN, [this](ui::Event* evt)
	{
		ui::MouseEvent* drag_event = static_cast<ui::MouseEvent*>(evt);
		anchor_ = drag_event->GetPosition(view());
		view()->widget()->SetCaptured();
	}).Listen(ui::EVENT_DRAG_MOVE, [this](ui::Event* evt)
	{
		POINT pt;
		::GetCursorPos(&pt);
		ui::Point position(pt);
		position.Offset(-anchor_.x(), -anchor_.y());
		view()->widget()->SetWindowPos(position.x(), position.y());
	}).Listen(ui::EVENT_DRAG_END, [this](ui::Event* evt)
	{
		::SetCapture(NULL);
	});

	
}

void TestWindowConstructor::OnInit()
{
	view()->SetWindowSize(400, 300);
	view()->CenterWindow();
	view()->RequestSetFocus();
}


ui::Window* TestWindowConstructor::view() const
{
	return window_;
}

