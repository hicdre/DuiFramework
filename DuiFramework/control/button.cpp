#include "stdafx.h"
#include "button.h"

#include "event/mouse_event.h"
#include "utils/image_store.h"
#include "layout/fill_layout.h"

namespace ui
{

	Button* Button::Create()
	{
		Button* btn = new Button;
		btn->Init(L"");
		return btn;
	}

	Button* Button::Create(const std::wstring& text)
	{
		Button* btn = new Button;
		btn->Init(text);
		return btn;
	}

	Button::Button()
	{
	}

	Button::~Button()
	{

	}

	void Button::Init(const std::wstring& text)
	{
		SetLayout(new FillLayout);
		for (int i = 0; i < STATE_MAX; i++)
		{
			ButtonStateView* state_view = new ButtonStateView((State)i, text);
			state_views[i] = state_view;

			Append(state_view);
		}

		SetState(NORMAL);
		UpdateButtonStateView();

		listener_.Listen(this, EVENT_MOUSE_ENTER,
			std::bind(&Button::OnMouseEnter, this, std::placeholders::_1, std::placeholders::_2));
		listener_.Listen(this, EVENT_MOUSE_LEAVE,
			std::bind(&Button::OnMouseLeave, this, std::placeholders::_1, std::placeholders::_2));
		listener_.Listen(this, EVENT_MOUSE_DOWN,
			std::bind(&Button::OnMouseDown, this, std::placeholders::_1, std::placeholders::_2));
		listener_.Listen(this, EVENT_MOUSE_UP,
			std::bind(&Button::OnMouseUp, this, std::placeholders::_1, std::placeholders::_2));
	}

	void Button::SetStateImage(State state, const std::string& id)
	{
		state_views[state]->set_background_image_id(id);
	}

	void Button::SetStateColor(State state, Color color)
	{
		state_views[state]->set_background_color(color);
	}

	void Button::SetState(State state)
	{
		if (state_ == state)
			return;
		state_ = state;
		UpdateButtonStateView();
		SchedulePaint();
	}

	Button::State Button::state() const
	{
		return state_;
	}

	View* Button::GetStateView(State state)
	{
		return state_views[state];
	}


	void Button::UpdateButtonStateView()
	{
		for (int i = 0; i < STATE_MAX; i++)
		{
			state_views[i]->SetVisible(i == state_);
		}
	}


	void Button::OnMouseEnter(View* v, Event* evt)
	{
		MouseEvent* mevt = static_cast<MouseEvent*>(evt);

		if (mevt->HasMouseDown())
		{
			SetState(Button::PRESSED);
		}
		else
		{
			SetState(Button::HOVERED);
		}
	}

	void Button::OnMouseLeave(View* v, Event* evt)
	{
		SetState(Button::NORMAL);
	}

	void Button::OnMouseDown(View* v, Event* evt)
	{
		SetState(Button::PRESSED);
	}

	void Button::OnMouseUp(View* v, Event* evt)
	{
		SetState(Button::HOVERED);
		DispatchClickEvent();
	}

	void Button::DispatchClickEvent()
	{
		Event evt(EVENT_BUTTON_CLICKED, this);
		DispatchPropagation(&evt);
	}

	void Button::SetTextFont(const Font& font)
	{
		for (int i = 0; i < STATE_MAX; i++)
		{
			state_views[i]->SetFont(font);
		}
	}

	void Button::SetTextFont(const std::wstring& name, int size)
	{
		for (int i = 0; i < STATE_MAX; i++)
		{
			state_views[i]->SetFont(name, size);
		}
	}

	void Button::SetTextHorizontalAlignment(HorizontalAlignment v)
	{
		for (int i = 0; i < STATE_MAX; i++)
		{
			state_views[i]->SetHorizontalAlignment(v);
		}
	}

	void Button::SetTextVerticalAlignment(VerticalAlignment v)
	{
		for (int i = 0; i < STATE_MAX; i++)
		{
			state_views[i]->SetVerticalAlignment(v);
		}
	}

	void Button::SetTextColor(Color color)
	{
		for (int i = 0; i < STATE_MAX; i++)
		{
			state_views[i]->SetTextColor(color);
		}
	}

	void Button::SetText(const std::wstring& text)
	{
		for (int i = 0; i < STATE_MAX; i++)
		{
			state_views[i]->SetText(text);
		}
	}


	ButtonStateView::ButtonStateView(Button::State state)
		: Label(), state_(state)
	{

	}

	ButtonStateView::ButtonStateView(Button::State state, const std::wstring& text)
		: Label(text), state_(state)
	{

	}

	Button::State ButtonStateView::GetButtonState() const
	{
		return state_;
	}

}