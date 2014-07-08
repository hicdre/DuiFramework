#pragma once
#include "core/label.h"
#include "event/event_listener.h"

namespace ui
{
	class Button : public Label
	{
	public:
		enum State {
			NORMAL,
			HOVERED,
			PRESSED,
		};
		class Delegate {
		public:
			virtual ~Delegate() {}
			virtual void SetStateImage(State state, const std::string& id) {}
			virtual void SetStateColor(State state, Color color) {}
			virtual void OnPaint(Button* btn, Painter* painter) {}
		};
		Button();
		Button(const std::wstring& text);
		virtual ~Button();

		void SetButtonDelegate(Delegate* delegate);

		void SetStateImage(State state, const std::string& id);
		void SetStateColor(State state, Color color);

		void SetState(State state);
		State state() const;

		void TriggerClicked();

		virtual void OnPaint(Painter* painter) override;
	private:
		State state_;
		scoped_ptr<Delegate> delegate_;
	};

	class DefaultButtonEventDelegate : public View::EventDelegate
	{
	public:
		virtual ~DefaultButtonEventDelegate() override;
		virtual void OnMouseEnter(View* v, Event* evt) override;
		virtual void OnMouseLeave(View* v, Event* evt) override;
		virtual void OnMouseDown(View* v, Event* evt) override;
		virtual void OnMouseUp(View* v, Event* evt) override;


	};

	class DefaultButtonDelegate : public Button::Delegate
	{
	public:
		virtual void SetStateImage(Button::State state, const std::string& id) override;
		virtual void SetStateColor(Button::State state, Color color) override;
		virtual void OnPaint(Button* btn, Painter* painter) override;

	private:
		struct StateData {
			std::string image_id;
			Color color{0xFFFFFFFF};
		} button_frames_[3];
	};

}