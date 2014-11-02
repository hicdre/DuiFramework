// Demo.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Demo.h"

#include "duiframework.h"

//#include "DemoWidget.h"
//#include "TestMouseEventWidget.h"
//#include "TestWidget.h"


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	ui::App* app = ui::App::Get();

	//ui::ImageStore::Default()->AddImageRecord("test", LR"(E:\work\xinyi\RomasterPC\bin\skin\avatar.png)");

	//ui::Widget* widget = ui::WidgetView::CreateWidget(new DemoWidget);

	ui::Window* window = new ui::Window(500, 300);
	{
		ui::Background* b = new ui::Background;
		{
// 			ui::Image* i = ui::Image::CreateLinearGradient(
// 				ui::Point(20, 60), ui::Point(20, 20), ui::Color_Green, ui::Color_Red);
// 			b->SetImage(i);
			b->SetColor(ui::Color_White);
		}
		window->SetBackground(b);
	}

	window->AttachWidget(ui::Widget::Create());
	window->widget()->Show(SW_SHOWNORMAL);

	app->Run();

	delete window;

	return 0;
}
