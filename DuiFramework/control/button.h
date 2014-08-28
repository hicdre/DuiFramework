#pragma once
#include "core/label.h"
#include "control/text_view.h"

namespace ui
{
	class Button : public TextView
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
		Button(const std::wstring& text);
		
		virtual ~Button();

		void SetStateImage(State state, const std::string& id);
		void SetStateColor(State state, Color color);

		void SetState(State state);
		State state() const;
		
		void TriggerClicked();
	protected:
		virtual void OnPaint(Painter* painter) override;

		virtual void OnMouseEnter(MouseEvent* evt) override;
		virtual void OnMouseLeave(MouseEvent* evt) override;
		virtual void OnMouseDown(MouseEvent* evt) override;
		virtual void OnMouseUp(MouseEvent* evt) override;
	private:
		class StateData;
		State state_;
		StateData* state_datas_[STATE_MAX];
	};
}