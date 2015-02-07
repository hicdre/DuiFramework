#pragma once
#include "foundation/foundation.h"
#include "uikit/common/ui_constants.h"

namespace ui
{
	class UIView;
	class UIMouse
	{
	public:
		enum Button {
			ButtonNone = 0,
			ButtonLeft = 1,
			ButtonMiddle = 2,
			ButtonRight = 4
		};

		UIMouse();

		void SetPositionInWindow(const Point& pt);
		int x() const;
		int y() const;
		const Point& positionInWindow() const;
		int deltaX() const;
		int deltaY() const;

		Button changedButton() const { return changed_button_; }
		void pressButton(Button b);
		void releaseButton(Button b);
		
		int buttons() const { return buttons_; }
		bool isLeftPressed() const { return !!(buttons_ & ButtonLeft); }
		bool isMiddlePressed() const { return !!(buttons_ & ButtonMiddle); }
		bool isRightPressed() const { return !!(buttons_ & ButtonRight); }

		void modifyCursor(CursorId id);
		CursorId cursor() const;

		void setView(UIView* view);
		UIView* view() const;
	protected:
		Point client_positon_;
		Point last_client_position_;
		Button changed_button_;
		int buttons_;
		CursorId cursor_;

		UIView* view_;

	};

}