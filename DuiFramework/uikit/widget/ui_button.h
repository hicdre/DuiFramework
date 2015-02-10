#pragma once
#include "uikit/widget/ui_control.h"
#include "uikit/widget/ui_label.h"

namespace ui
{
	class UIButton : public UIControl
	{
	public:
		UILabel* titleLabel();

		std::wstring titleForState(UIControl::State states) const;
		void setTitleforState(const std::wstring& str, UIControl::State state);

		Color titleColorForState(UIControl::State states) const;
		void setTitleColorForState(Color color, UIControl::State states);

		

		std::wstring currentTitle() const;
		Color currentTitleColor() const;
	protected:
		virtual ~UIButton() override;
		virtual void mousePress(UIMouse* mouse, UIEvent* event) override;
		virtual void mouseRelease(UIMouse* mouse, UIEvent* event) override;

		virtual void OnPressedChanged() override;
		virtual void OnHoveredChanged() override;

		virtual void OnPaint(UIRenderContext* painter) override;

		virtual void willMoveToWindow(UIWindow* window) override;

		void UpdateCurrentAttributes();

		UILabel* titleLabel_{ NULL };

		std::unordered_map<UIControlState, std::wstring> stateStrings_;
		std::unordered_map<UIControlState, Color> stateColors_;

	};
}