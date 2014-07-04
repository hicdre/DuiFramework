#include "stdafx.h"
#include "TestMouseEventWidget.h"
#include "core/widget_view.h"
#include "core/label.h"

void TestMouseEventWidget::OnInit()
{
	{
		labe_ = new ui::Label(L"Hello world!");
		labe_->SetBounds(0, 0, 100, 50);
		labe_->SetFont(L"Consolas", 12);
		labe_->SetHorizontalAlignment(ui::ALIGN_LEFT);
		labe_->SetTextColor(ui::ColorSetRGB(0, 0, 0));
		view()->Append(labe_);
	}

	{
		ui::View* child_view = new ui::View;
		child_view->SetBounds(100, 0, 300, 300);
		child_view->set_background_color(ui::ColorSetRGB(0, 255, 0));
		child_view->RegisterEventListener(this);
		view()->Append(child_view);
	}

	
	view()->set_background_color(ui::ColorSetRGB(255, 255, 255));

	view()->CenterWidget();
}

ui::Rect TestMouseEventWidget::GetInitialRect()
{
	return ui::Rect(0, 0, 400, 300);
}

void TestMouseEventWidget::OnMouseMove(ui::View* v, ui::MouseEvent& event)
{
	ui::Point pt = event.GetPosition();
	std::wstring text = L"{" + std::to_wstring(pt.x()) + L", " + std::to_wstring(pt.y()) + L"}";
	labe_->SetText(text);
}
