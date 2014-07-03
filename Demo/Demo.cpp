// Demo.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Demo.h"

#include "core/app.h"
#include "core/widget_view.h"

#include "DemoWidget.h"


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	ui::App* app = ui::App::Get();

	ui::ImageStore::Default()->AddImageRecord("test", LR"(E:\work\xinyi\RomasterPC\bin\skin\avatar.png)");

	ui::Widget* widget = ui::WidgetView::CreateWidget(new DemoWidget);
	widget->Show(SW_SHOWNORMAL);

	app->Run();

	delete widget;

	return 0;
}
