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
	/*std::string css_string = "div { background: url(\"something.png\"); }";
	ui::StyleScanner reader(css_string, 0);
	ui::StyleToken token;
	while (reader.Next(token, true))
	{
		ui::StyleTokenType type = token.mType;
	}*/

	ui::Window* window = new ui::Window;
	window->LoadFile(L"test.xml");
	window->AttachWidget(ui::Widget::Create());
	window->widget()->Show(SW_SHOWNORMAL);
	window->CenterWindow();

	app->Run();

	delete window;

	return 0;
}
