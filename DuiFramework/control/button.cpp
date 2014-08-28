#include "stdafx.h"
#include "button.h"

#include "event/mouse_event.h"
#include "core/image.h"
#include "core/rectangle.h"

namespace ui
{
	class Button::StateData
	{
	public:
		StateData(Button::State state)
			: state_(state)
		{
		}

		void SetImage(const std::string& id) {
			state_graphics_.reset(new ResourceImage(id));
		}

		void SetSolidColor(Color color) {
			state_graphics_.reset(new SolidRectangle(color));
		}

		Button::State GetButtonState() const {
			return state_;
		}

		Drawable* GetData() const {
			return state_graphics_.get();
		}
	private:
		Button::State state_;
		scoped_ptr<Drawable> state_graphics_;
	};

	Button::Button()
	{
		for (int i = 0; i < STATE_MAX; i++)
		{
			state_datas_[i] = new StateData((State)i);
		}
	}

	Button::Button(const std::wstring& text)
		: TextView(text)
	{
		for (int i = 0; i < STATE_MAX; i++)
		{
			state_datas_[i] = new StateData((State)i);
		}
	}

	Button::~Button()
	{
		for (int i = 0; i < STATE_MAX; i++)
		{
			delete state_datas_[i];
		}
	}

	void Button::SetStateImage(State state, const std::string& id)
	{
		state_datas_[state]->SetImage(id);
	}

	void Button::SetStateColor(State state, Color color)
	{
		state_datas_[state]->SetSolidColor(color);
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
		Drawable* graphics = state_datas_[state_]->GetData();
		if (graphics)
			graphics->DoPaint(painter, GetContentsBounds());
	}


	void Button::OnMouseEnter(MouseEvent* evt)
	{
		if (evt->HasMouseDown())
		{
			SetState(Button::PRESSED);
		}
		else
		{
			SetState(Button::HOVERED);
		}
	}

	void Button::OnMouseLeave(MouseEvent* evt)
	{
		SetState(Button::NORMAL);
	}

	void Button::OnMouseDown(MouseEvent* evt)
	{
		SetState(Button::PRESSED);
	}

	void Button::OnMouseUp(MouseEvent* evt)
	{
		SetState(Button::HOVERED);
		TriggerClicked();
	}


	void Button::TriggerClicked()
	{
		Clicked evt(this);
		HandleEvent(&evt);
	}


	Button::Clicked::Clicked(Button* btn)
		: Event(EVENT_BUTTON_CLICKED)
		, trigger_(btn)
	{

	}

}