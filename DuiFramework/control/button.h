#pragma once
#include "core/label.h"
#include "core/complex_view.h"

namespace ui
{
#if 0
	class ButtonStateView;

	/*
		<Button>
		  <Button::State state="normal" inhert="Label" />
		  <Button::State state="hovered" inhert="Label" />
		  <Button::State state="pressed" inhert="Label" />
		</Button>
	*/
	class Button : public ComplexView
	{
	public:
		enum State {
			NORMAL,
			HOVERED,
			PRESSED,

			STATE_MAX,
		};
		static Button* Create();
		static Button* Create(const std::wstring& text);
		
		virtual ~Button();

		void SetStateImage(State state, const std::string& id);
		void SetStateColor(State state, Color color);

		void SetState(State state);
		State state() const;

		View* GetStateView(State state);

		void SetText(const std::wstring& text);
		void SetTextFont(const Font& font);
		void SetTextFont(const std::wstring& name, int size);
		void SetTextHorizontalAlignment(HorizontalAlignment i);
		void SetTextVerticalAlignment(VerticalAlignment i);
		void SetTextColor(Color color);
		
	protected:
		Button();
		void Init(const std::wstring& text);

		void UpdateButtonStateView();
	private:
		void OnMouseEnter(View* v, Event* evt);
		void OnMouseLeave(View* v, Event* evt);
		void OnMouseDown(View* v, Event* evt);
		void OnMouseUp(View* v, Event* evt);

		void DispatchClickEvent();

		State state_;
		ButtonStateView* state_views[STATE_MAX];
	};
#endif
}