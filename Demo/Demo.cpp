// Demo.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Demo.h"

#include "core/app.h"
#include "core/widget.h"
#include "core/view.h"
#include "render/rect.h"


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	ui::App* app = ui::App::Get();

	ui::Widget* widget = ui::Widget::Create(NULL, ui::Rect(100,100,200,200));
	widget->SetCursor(::LoadCursor(NULL, IDC_ARROW));
	{
		ui::View* root_view = new ui::View;
		{
			ui::View* child_view = new ui::View;
			child_view->SetBounds(100, 50, 50, 100);
			child_view->set_background_color(ui::ColorSetRGB(0, 255, 0));
			root_view->Append(child_view);
		}
		{
			ui::View* child_view = new ui::View;
			child_view->SetBounds(0, 50, 50, 50);
			{
				ui::Border* border = new ui::Border;
				border->SetBorder(1, ui::ColorSetRGB(255, 0, 0));
				child_view->SetBorder(border);
			}
			root_view->Append(child_view);
		}
		{
			ui::View* child_view = new ui::View;
			child_view->SetBounds(0, 100, 50, 50);
			{
				ui::Border* border = new ui::Border;
				border->SetBorder(ui::Border::LEFT, 2, ui::ColorSetRGB(0, 255, 0));
				border->SetBorder(ui::Border::RIGHT, 2, ui::ColorSetRGB(0, 255, 0));
				border->SetBorder(ui::Border::BOTTOM, 2, ui::ColorSetRGB(0, 255, 0));
				child_view->SetBorder(border);
			}
			root_view->Append(child_view);
		}
		{
			ui::View* child_view = new ui::View;
			child_view->SetBounds(0, 150, 50, 50);
			child_view->set_background_color(ui::ColorSetRGB(0, 255, 255));
			{
				ui::Border* border = new ui::Border;
				border->SetBorder(ui::Border::TOP, 10, ui::ColorSetRGB(125, 255, 0));
				border->SetBorder(ui::Border::BOTTOM, 2, ui::ColorSetRGB(0, 100, 125));
				child_view->SetBorder(border);
			}
			root_view->Append(child_view);
		}
		root_view->set_background_color(ui::ColorSetRGB(255, 255, 0));
		widget->SetView(root_view);
	}
	widget->Show(SW_SHOWNORMAL);

	app->Run();

	return 0;
}
