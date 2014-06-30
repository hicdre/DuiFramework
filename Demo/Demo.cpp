// Demo.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Demo.h"

#include "core/app.h"
#include "core/widget.h"
#include "render/rect.h"


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	ui::App* app = ui::App::Get();

	ui::Widget* widget = ui::Widget::Create(NULL, ui::Rect(100,100,200,100));
	widget->SetCursor(::LoadCursor(NULL, IDC_ARROW));
	widget->Show(SW_SHOWNORMAL);

	app->Run();

	return 0;
}
