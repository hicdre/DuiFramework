#include "stdafx.h"
#include "TestMouseEventWidget.h"
#include "core/widget_view.h"
#include "core/label.h"

std::wstring PointToString(const ui::Point& pt)
{
	std::wstring text = L"{" + std::to_wstring(pt.x()) + L", " + std::to_wstring(pt.y()) + L"}";
	return text;
}

void TestMouseEventWidget::OnInit()
{
	green_model_.SetObserver(this);
	red_model_.SetObserver(this);
	{
		ui::View* green_view = new ui::View;
		view()->Append(green_view);

		green_view->SetBounds(100, 0, 300, 300);
		green_view->set_background_color(ui::ColorSetRGB(0, 255, 0));
		listener_.Listen(green_view, ui::EVENT_MOUSE_MOVE,
			std::bind(&TestMouseEventWidget::OnGreenViewMove, this, std::placeholders::_1));
		listener_.Listen(green_view, ui::EVENT_MOUSE_ENTER, [this](ui::Event* evt)
		{
			green_model_.SetMouseIn(true);
		});
		listener_.Listen(green_view, ui::EVENT_MOUSE_LEAVE, [this](ui::Event* evt)
		{
			green_model_.SetMouseIn(false);
		});

		{
			green_labe_ = new ui::Label(L"Green View");
			green_labe_->SetBounds(0, 0, 100, 50);
			green_labe_->SetFont(L"Consolas", 12);
			green_labe_->SetHorizontalAlignment(ui::ALIGN_LEFT);
			green_labe_->SetTextColor(ui::ColorSetRGB(0, 0, 0));
			green_view->Append(green_labe_);
		}
		{
			ui::View* red_view = new ui::View;
			green_view->Append(red_view);
			red_view->SetBounds(20, 50, 200, 200);
			red_view->set_background_color(ui::ColorSetRGB(255, 0, 0));
			listener_.Listen(red_view, ui::EVENT_MOUSE_MOVE,
				std::bind(&TestMouseEventWidget::OnRedViewMove, this, std::placeholders::_1));
			listener_.Listen(red_view, ui::EVENT_MOUSE_ENTER, [this](ui::Event* evt)
			{
				red_model_.SetMouseIn(true);
			});
			listener_.Listen(red_view, ui::EVENT_MOUSE_LEAVE, [this](ui::Event* evt)
			{
				red_model_.SetMouseIn(false);
			});
			{
				red_labe_ = new ui::Label(L"Red View");
				red_labe_->SetBounds(0, 0, 100, 50);
				red_labe_->SetFont(L"Consolas", 12);
				red_labe_->SetHorizontalAlignment(ui::ALIGN_LEFT);
				red_labe_->SetTextColor(ui::ColorSetRGB(0, 0, 0));
				red_view->Append(red_labe_);
			}
		}
	}

	
	view()->set_background_color(ui::ColorSetRGB(255, 255, 255));

	view()->CenterWidget();
}

ui::Rect TestMouseEventWidget::GetInitialRect()
{
	return ui::Rect(0, 0, 400, 300);
}


void TestMouseEventWidget::OnGreenViewMove(ui::Event* event)
{
	ui::Point pt = static_cast<ui::MouseEvent*>(event)->GetPosition();
	green_model_.SetPoint(pt);
}

void TestMouseEventWidget::OnRedViewMove(ui::Event* event)
{
	ui::Point pt = static_cast<ui::MouseEvent*>(event)->GetPosition();
	red_model_.SetPoint(pt);
}

void TestMouseEventWidget::OnModelChanged(TestMouseModel* model)
{
	if (model == &green_model_)
	{
		green_labe_->SetText(green_model_.ToString());
	}
	else if (model == &red_model_)
	{
		red_labe_->SetText(red_model_.ToString());
	}
}



TestMouseModel::TestMouseModel()
{

}

TestMouseModel::~TestMouseModel()
{

}


void TestMouseModel::SetObserver(Observer* ob)
{
	ob_ = ob;
}


void TestMouseModel::SetPoint(const ui::Point& pt)
{
	if (pt != pt_)
	{
		pt_ = pt;
		if (ob_)
			ob_->OnModelChanged(this);
	}
}

void TestMouseModel::SetMouseIn(bool v)
{
	if (v != is_mouse_in_)
	{
		is_mouse_in_ = v;
		if (ob_)
			ob_->OnModelChanged(this);
	}
}

std::wstring TestMouseModel::ToString()
{
	std::wstring text = PointToString(pt_);
	if (is_mouse_in_)
		text += L" in";
	else
		text += L" out";
	return text;
}
