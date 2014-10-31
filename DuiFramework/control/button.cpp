#include "stdafx.h"
#include "button.h"

#include "event/mouse_event.h"
#include "core/image.h"
#include "core/rectangle.h"

namespace ui
{
	Button::Clicked::Clicked(Button* btn)
		: Event(EVENT_BUTTON_CLICKED)
		, trigger_(btn)
	{

	}


	Button::Button()
	{

	}


	Button::~Button()
	{

	}


	void Button::SetState(State state)
	{
		if (state_ == state)
			return;
		state_ = state;
		OnButtonStateChanged();
	}

	Button::State Button::state() const
	{
		return state_;
	}


	class TextButton::StateData
	{
	public:
		StateData(TextButton::State state)
			: state_(state)
		{
		}

		void SetImage(const std::string& id) {
			state_graphics_.reset(new ResourceImage(id));
		}

		void SetSolidColor(Color color) {
			state_graphics_.reset(new SolidRectangle(color));
		}

		TextButton::State GetButtonState() const {
			return state_;
		}

		Drawable* GetData() const {
			return state_graphics_.get();
		}
	private:
		TextButton::State state_;
		scoped_ptr<Drawable> state_graphics_;
	};

	TextButton::TextButton()
	{
		for (int i = 0; i < STATE_MAX; i++)
		{
			state_datas_[i] = new StateData((State)i);
		}
	}

	TextButton::TextButton(const std::wstring& text)
		: TextView(text)
	{
		for (int i = 0; i < STATE_MAX; i++)
		{
			state_datas_[i] = new StateData((State)i);
		}
	}

	TextButton::~TextButton()
	{
		for (int i = 0; i < STATE_MAX; i++)
		{
			delete state_datas_[i];
		}
	}

	void TextButton::SetStateImage(State state, const std::string& id)
	{
		state_datas_[state]->SetImage(id);
	}

	void TextButton::SetStateColor(State state, Color color)
	{
		state_datas_[state]->SetSolidColor(color);
	}



	void TextButton::OnPaint(RenderContext* painter)
	{
		Drawable* graphics = state_datas_[state_]->GetData();
		if (graphics)
			graphics->DoPaint(painter, GetContentsBounds());
	}


	void TextButton::OnMouseEnter(MouseEvent* evt)
	{
		if (evt->HasMouseDown())
		{
			SetState(TextButton::PRESSED);
		}
		else
		{
			SetState(TextButton::HOVERED);
		}
	}

	void TextButton::OnMouseLeave(MouseEvent* evt)
	{
		SetState(TextButton::NORMAL);
	}

	void TextButton::OnMouseDown(MouseEvent* evt)
	{
		SetState(TextButton::PRESSED);
	}

	void TextButton::OnMouseUp(MouseEvent* evt)
	{
		SetState(TextButton::HOVERED);
		TriggerClicked();
	}


	void TextButton::TriggerClicked()
	{
		Clicked evt(this);
		HandleEvent(&evt);
	}

	void TextButton::OnButtonStateChanged()
	{
		SchedulePaint();
	}

}