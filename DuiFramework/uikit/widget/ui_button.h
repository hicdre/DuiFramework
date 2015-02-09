#pragma once
#include "uikit/widget/ui_control.h"
#include "uikit/widget/ui_label.h"

namespace ui
{
	class UIButton : public UIControl
	{
	public:
		UILabel* titleLabe();
		std::wstring titleForState(UIControlState state) const;
		void setTitleforState(const std::wstring& str, UIControlState state);
// 		void attributedTitleForState(UIControlState state);
// 		void setAttributedTitleForState(UIControlState state);
		Color titleColorForState(UIControlState state);
		void setTitleColorForState(Color color, UIControlState state);
// 		void titleShadowColorForState(UIControlState state);
// 		void setTitleShadowColorForState(UIControlState state);

		virtual void OnPaint(UIRenderContext* painter) override;
	protected:
		virtual ~UIButton() override;
		virtual void mouseMove(UIMouse* mouse, UIEvent* event) override;
		virtual void mousePress(UIMouse* mouse, UIEvent* event) override;
		virtual void mouseRelease(UIMouse* mouse, UIEvent* event) override;


		UILabel* titleLabel_{ NULL };
	};
}