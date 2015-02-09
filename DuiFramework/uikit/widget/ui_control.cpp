#include "stdafx.h"
#include "ui_control.h"

namespace ui
{


	void UIControl::setEnabled(bool v)
	{
		if (enabled_ == v)
			return;
		enabled_ = v;
		OnEnabledChanged();
	}

	void UIControl::setSelected(bool v)
	{
		if (selected_ == v)
			return;
		selected_ = v;
		OnSelectedChanged();
	}


	void UIControl::setHovered(bool v)
	{
		if (hovered_ == v)
			return;
		hovered_ = v;
		OnHoveredChanged();
	}


	void UIControl::setPressed(bool v)
	{
		if (pressed_ == v)
			return;
		pressed_ = v;
		OnPressedChanged();
	}

	void UIControl::setContentVerticalAlignment(UIControlContentVerticalAlignment v)
	{
		if (contentVerticalAlignment_ == v)
			return;
		contentVerticalAlignment_ = v;
	}

	void UIControl::setContentHorizontalAlignment(UIControlContentHorizontalAlignment v)
	{
		if (contentHorizontalAlignment_ == v)
			return;
		contentHorizontalAlignment_ = v;
	}

	UIControl::State UIControl::state() const
	{
		State s = UIControlStateNormal;
		if (!isEnabled())
			s |= UIControlStateDisabled;
		if (!isSelected())
			s |= UIControlStateSelected;
		if (!isHovered())
			s |= UIControlStateHovered;
		if (!isPressed())
			s |= UIControlStatePressed;
		return s;
	}

	void UIControl::addTargetForControlEvents(UIObject* obj, const Selector& s, Events events)
	{
		for (ActionTargetItem& item : actionTargetTable_)
		{
			if (item.obj == obj && item.sel == s)
			{
				item.events |= events;
				return;
			}
		}
		ActionTargetItem newItem;
		newItem.obj = obj;
		newItem.sel = s;
		newItem.events = events;
		actionTargetTable_.push_back(std::move(newItem));
	}

	void UIControl::removeTargetForControlEvents(UIObject* obj, const Selector& s, Events events)
	{
		for (auto iter = actionTargetTable_.begin(); iter != actionTargetTable_.end(); ++iter)
		{
			if (iter->obj == obj && iter->sel == s)
			{
				iter->events &= ~events;
				if (iter->events == 0) {
					actionTargetTable_.erase(iter);
				}
				return;
			}
		}
	}

	void UIControl::sendActionsForControlEvents(Events events)
	{
		for (auto iter = actionTargetTable_.begin(); iter != actionTargetTable_.end(); ++iter)
		{
			if (iter->events & events)
			{
				iter->obj->performSelector(iter->sel);
			}
		}
	}

	void UIControl::OnEnabledChanged()
	{

	}

	void UIControl::OnSelectedChanged()
	{

	}

	void UIControl::OnHoveredChanged()
	{

	}

	void UIControl::OnPressedChanged()
	{

	}

}