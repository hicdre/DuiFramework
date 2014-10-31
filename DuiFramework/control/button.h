#pragma once
#include "control/text_view.h"

namespace ui
{
	class Button
	{
	public:
		enum State {
			NORMAL,
			HOVERED,
			PRESSED,

			STATE_MAX,
		};

		class Clicked : public Event
		{
		public:
			Clicked(Button* btn);

			Button* trigger() const { return trigger_; }
		protected:
			Button* trigger_;
		};

		Button();
		virtual ~Button();

		virtual void TriggerClicked() = 0;

		virtual void OnButtonStateChanged() = 0;

		void SetState(State state);
		State state() const;
	protected:
		State state_;
	};

	class TextButton 
		: public TextView
		, public Button
	{
	public:
		TextButton();
		TextButton(const std::wstring& text);
		
		virtual ~TextButton();

		//@attr
		void SetStateImage(State state, const std::string& id);
		void SetStateColor(State state, Color color);
		
		virtual void TriggerClicked() override;
		virtual void OnButtonStateChanged() override;
	protected:
		virtual void OnPaint(RenderContext* painter) override;

		virtual void OnMouseEnter(MouseEvent* evt) override;
		virtual void OnMouseLeave(MouseEvent* evt) override;
		virtual void OnMouseDown(MouseEvent* evt) override;
		virtual void OnMouseUp(MouseEvent* evt) override;
	private:
		class StateData;
		StateData* state_datas_[STATE_MAX];
	};
}