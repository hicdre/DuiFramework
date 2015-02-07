#include "stdafx.h"
#include "ui_mouse.h"
#include <cassert>

namespace ui
{


	UIMouse::UIMouse()
		: client_positon_(-1, -1)
		, changed_button_(ButtonNone)
		, buttons_(0)
		, cursor_(Cursor_Arrow)
		, view_(NULL)
	{

	}

	const Point& UIMouse::positionInWindow() const
	{
		return client_positon_;
	}

	int UIMouse::x() const
	{
		return client_positon_.x();
	}

	int UIMouse::y() const
	{
		return client_positon_.y();
	}

	void UIMouse::SetPositionInWindow(const Point& pt)
	{
		if (client_positon_ == pt)
			return;
		last_client_position_ = client_positon_;
		client_positon_ = pt;
	}

	int UIMouse::deltaX() const
	{
		return client_positon_.x() - last_client_position_.x();
	}

	int UIMouse::deltaY() const
	{
		return client_positon_.y() - last_client_position_.y();
	}

	void UIMouse::pressButton(Button b)
	{
		changed_button_ = b;
		buttons_ |= b;
	}

	void UIMouse::releaseButton(Button b)
	{
		changed_button_ = b;
		buttons_ &= ~b;
	}

	void UIMouse::modifyCursor(CursorId id)
	{
		cursor_ = id;
	}

	ui::CursorId UIMouse::cursor() const
	{
		return cursor_;
	}

	void UIMouse::setView(UIView* view)
	{
		view_ = view;
	}

	UIView* UIMouse::view() const
	{
		return view_;
	}

}