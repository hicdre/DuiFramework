#include "stdafx.h"
#include "ui_button.h"

namespace ui
{

	UIButton::~UIButton()
	{
	}


	UILabel* UIButton::titleLabel()
	{
		if (titleLabel_ == NULL){
			titleLabel_ = new UILabel;
			titleLabel_->InitWithBounds(GetLocalBounds());
			titleLabel_->setTextAlignment(TextAlignmentCenter);
			addSubView(titleLabel_);
		}
		return titleLabel_;
	}

	std::wstring UIButton::titleForState(UIControl::State state) const
	{
		uint32 i = UIControlStateSelected;
		while (i) {
			if ((state & i) && stateStrings_.count((UIControlState)i))
				return stateStrings_.at((UIControlState)i);
			i >>= 1;
		}
		return stateStrings_.at(UIControlStateNormal);
	}

	void UIButton::setTitleforState(const std::wstring& str, UIControl::State state)
	{
		if (state == UIControlStateNormal) {
			stateStrings_[UIControlStateNormal] = str;
			return;
		}

		uint32 i = UIControlStateSelected;
		while (i) {
			if (state & i)
				stateStrings_[(UIControlState)i] = str;
			i >>= 1;
		}
	}

	Color UIButton::titleColorForState(UIControl::State state) const
	{
		uint32 i = UIControlStateSelected;
		while (i) {
			if ((state & i) && stateColors_.count((UIControlState)i))
				return stateColors_.at((UIControlState)i);
			i >>= 1;
		}
		return stateColors_.at(UIControlStateNormal);
	}

	void UIButton::setTitleColorForState(Color color, UIControl::State state)
	{
		if (state == UIControlStateNormal) {
			stateColors_[UIControlStateNormal] = color;
			return;
		}

		uint32 i = UIControlStateSelected;
		while (i) {
			if (state & i)
				stateColors_[(UIControlState)i] = color;
			i >>= 1;
		}
	}

	void UIButton::mousePress(UIMouse* mouse, UIEvent* event)
	{
		setPressed(true);
		sendActionsForControlEvents(UIControlEventClick);
	}

	void UIButton::mouseRelease(UIMouse* mouse, UIEvent* event)
	{
		setPressed(false);
	}

	void UIButton::OnPaint(UIRenderContext* painter)
	{
		if (background_color_ != Color_Transparent)
			painter->FillRect(GetLocalBounds(), background_color_);

		titleLabel()->DoPaint(painter, GetLocalBounds());
	}

	void UIButton::UpdateCurrentAttributes()
	{
		titleLabel()->setText(currentTitle());
		titleLabel()->setTextColor(currentTitleColor());
	}

	std::wstring UIButton::currentTitle() const
	{
		return titleForState(state());
	}

	Color UIButton::currentTitleColor() const
	{
		return titleColorForState(state());
	}

	void UIButton::OnPressedChanged()
	{
		UpdateCurrentAttributes();
	}

	void UIButton::OnHoveredChanged()
	{
		UpdateCurrentAttributes();
	}

	void UIButton::willMoveToWindow(UIWindow* window)
	{
		if (window) {
			UpdateCurrentAttributes();
		}
	}

}