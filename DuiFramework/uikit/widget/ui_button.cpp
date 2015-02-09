#include "stdafx.h"
#include "ui_button.h"

namespace ui
{

	UIButton::~UIButton()
	{
		if (titleLabel_)
		{
			delete titleLabel_;
			titleLabel_ = NULL;
		}
	}


	UILabel* UIButton::titleLabe()
	{
		if (titleLabel_ == NULL){
			titleLabel_ = new UILabel;
			titleLabel_->InitWithBounds(GetLocalBounds());
		}
		return titleLabel_;
	}

	std::wstring UIButton::titleForState(UIControlState state) const
	{
		return L"";
	}

	void UIButton::setTitleforState(const std::wstring& str, UIControlState state)
	{
		titleLabe()->setText(str);
	}

	Color UIButton::titleColorForState(UIControlState state)
	{
		return Color_Black;
	}

	void UIButton::setTitleColorForState(Color color, UIControlState state)
	{

	}

	void UIButton::mouseMove(UIMouse* mouse, UIEvent* event)
	{
		setHovered(true);
	}

	void UIButton::mousePress(UIMouse* mouse, UIEvent* event)
	{
		sendActionsForControlEvents(UIControlEventClick);
	}

	void UIButton::mouseRelease(UIMouse* mouse, UIEvent* event)
	{

	}

	void UIButton::OnPaint(UIRenderContext* painter)
	{
		UIControl::OnPaint(painter);

		titleLabe()->DoPaint(painter, GetLocalBounds());
	}

}