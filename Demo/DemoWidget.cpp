#include "stdafx.h"
#include "DemoWidget.h"
#include "core/widget_view.h"
#include "core/label.h"

void DemoWidget::OnInit()
{
	{
		ui::View* child_view = new ui::View;
		child_view->SetBounds(100, 50, 50, 100);
		child_view->set_background_color(ui::ColorSetRGB(0, 255, 0));
		view()->Append(child_view);
	}
	{
		ui::View* child_view = new ui::View;
		child_view->SetBounds(0, 50, 50, 50);
		{
			ui::NormalBorder* border = new ui::NormalBorder;
			border->SetBorder(1, ui::ColorSetRGB(255, 0, 0));
			child_view->SetBorder(border);
		}
		view()->Append(child_view);
	}
	{
		ui::View* child_view = new ui::View;
		child_view->SetBounds(0, 100, 50, 50);
		{
			ui::NormalBorder* border = new ui::NormalBorder;
			border->SetBorder(ui::NormalBorder::LEFT, 2, ui::ColorSetRGB(0, 255, 0));
			border->SetBorder(ui::NormalBorder::RIGHT, 2, ui::ColorSetRGB(0, 255, 0));
			border->SetBorder(ui::NormalBorder::BOTTOM, 2, ui::ColorSetRGB(0, 255, 0));
			child_view->SetBorder(border);
		}
		view()->Append(child_view);
	}
	{
		ui::View* child_view = new ui::View;
		child_view->SetBounds(0, 150, 50, 50);
		child_view->set_background_color(ui::ColorSetRGB(0, 255, 255));
		{
			ui::NormalBorder* border = new ui::NormalBorder;
			border->SetBorder(ui::NormalBorder::TOP, 10, ui::ColorSetRGB(125, 255, 0));
			border->SetBorder(ui::NormalBorder::BOTTOM, 2, ui::ColorSetRGB(0, 100, 125));
			child_view->SetBorder(border);
		}
		view()->Append(child_view);
	}
	{
		ui::Label* label = new ui::Label(L"Hello world!");
		label->SetBounds(0, 0, 100, 50);
		label->SetFont(L"Consolas", 14);
		label->SetHorizontalAlignment(ui::ALIGN_CENTER);
		label->SetTextColor(ui::ColorSetRGB(0, 255, 0));
		view()->Append(label);
	}
	view()->set_background_image_id("test");
}

ui::Rect DemoWidget::GetInitialRect()
{
	return ui::Rect(100, 100, 200, 200);
}
