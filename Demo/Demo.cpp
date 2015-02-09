// Demo.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Demo.h"

#include "AppDelegate.h"
#include "UIRangeTest.h"
//#include "utils/utils.h"

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

// 	ui::App* app = ui::App::Get();
// 	std::wstring app_dir = ui::dirname(app->GetAppPath());
// 	app->GetResourceLoader()->Rebase(ui::pathcombine(app_dir, L"../../../Demo/res/"));
// 
// 	ui::UIDocumentPtr document =
// 		app->GetResourceLoader()->GetUIDocument(ui::URL("res://local/test.xml"));
// 
// 	document->RootElement()->AddEventListener(ui::EVENT_MOUSE_DOWN, ui::CPPEventListener::Create(
// 		[app](ui::Event* event)
// 	{
// 		ui::UIElement* element = dynamic_cast<ui::UIElement*>(event->target());
// 		if (element && element->getId() == "close") {
// 			app->Quit();
// 		}
// 	}));
// 	document->CreateWidget()->Show(SW_SHOWNORMAL);
// 	//ui::ImageStore::Default()->AddImageRecord("test", LR"(E:\work\xinyi\RomasterPC\bin\skin\avatar.png)");
// // 	ui::Window* window = new ui::Window;
// // 	window->LoadFile(L"test.xml");
// // 	window->AttachWidget(ui::Widget::Create());
// // 	window->widget()->Show(SW_SHOWNORMAL);
// // 	window->CenterWindow();
// 
// 	app->Run();
// 
// //	delete window;

//	RunUikitRangeTest();
	ui::UIApplication::Main(new AppDelegate);

	return 0;
}
