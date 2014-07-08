#include "stdafx.h"
#include "button.h"

#include "event/mouse_event.h"
#include "utils/image_store.h"

namespace ui
{

	Button::Button(const std::wstring& text)
		: Label(text)
	{
		SetEventDelegate(new DefaultButtonEventDelegate);
		SetButtonDelegate(new DefaultButtonDelegate);
	}

	Button::Button()
		: Button(L"")
	{

	}

	Button::~Button()
	{

	}

	void Button::SetStateImage(State state, const std::string& id)
	{
		if (delegate_.get())
		{
			delegate_->SetStateImage(state, id);
		}
	}

	void Button::SetStateColor(State state, Color color)
	{
		if (delegate_.get())
		{
			delegate_->SetStateColor(state, color);
		}
	}

	void Button::SetState(State state)
	{
		if (state_ == state)
			return;
		state_ = state;
		SchedulePaint();
	}

	Button::State Button::state() const
	{
		return state_;
	}

	void Button::OnPaint(Painter* painter)
	{
		if (delegate_.get())
		{
			delegate_->OnPaint(this, painter);
		}
		Label::OnPaint(painter);
	}

	void Button::SetButtonDelegate(Delegate* delegate)
	{
		delegate_.reset(delegate);
	}

	void Button::TriggerClicked()
	{
		Event evt(EVENT_BUTTON_CLICKED, this);
		GetEventDispatcher()->DispatchPropagation(&evt, this);
	}



	DefaultButtonEventDelegate::~DefaultButtonEventDelegate()
	{

	}

	void DefaultButtonEventDelegate::OnMouseEnter(View* v, Event* evt)
	{
		Button* btn = static_cast<Button*>(v);
		MouseEvent* mevt = static_cast<MouseEvent*>(evt);

		if (mevt->HasMouseDown())
		{
			btn->SetState(Button::PRESSED);
		}
		else
		{
			btn->SetState(Button::HOVERED);
		}
	}

	void DefaultButtonEventDelegate::OnMouseLeave(View* v, Event* evt)
	{
		Button* btn = static_cast<Button*>(v);
		btn->SetState(Button::NORMAL);
	}

	void DefaultButtonEventDelegate::OnMouseDown(View* v, Event* evt)
	{
		Button* btn = static_cast<Button*>(v);
		btn->SetState(Button::PRESSED);
	}

	void DefaultButtonEventDelegate::OnMouseUp(View* v, Event* evt)
	{
		Button* btn = static_cast<Button*>(v);
		btn->SetState(Button::HOVERED);
		btn->TriggerClicked();
	}


	void DefaultButtonDelegate::SetStateImage(Button::State state, const std::string& id)
	{
		button_frames_[state].image_id = id;
	}

	void DefaultButtonDelegate::SetStateColor(Button::State state, Color color)
	{
		button_frames_[state].color = color;
	}

	void DefaultButtonDelegate::OnPaint(Button* btn, Painter* painter)
	{
		Button::State state = btn->state();
		StateData& d = button_frames_[state];
		if (!d.image_id.empty())
		{
			ImageClip* clip = ImageStore::Default()->GetImageById(d.image_id);
			if (clip) {
				painter->DrawImage(clip, btn->GetContentBounds());
				return;
			}
		}

		painter->FillRect(btn->GetContentBounds(), d.color);
	}

}