#pragma once
#include "event/input_event.h"

namespace ui
{
	class View;
	class MouseEvent : public InputEvent
	{
	public:

		enum Button {
			ButtonNone = -1,
			ButtonLeft,
			ButtonMiddle,
			ButtonRight
		};

		MouseEvent();

		void setScreenLocation(int x, int y) { m_screenLocation.SetPoint(x, y); }
		int screenX() const { return m_screenLocation.x(); }
		int screenY() const { return m_screenLocation.y(); }
		const Point& screenLocation() const { return m_screenLocation; }

		void setClientLocation(int x, int y) { m_clientLocation.SetPoint(x, y); }
		int clientX() const { return m_clientLocation.x(); }
		int clientY() const { return m_clientLocation.y(); }

		int movementX() const { return m_movementDelta.x(); }
		int movementY() const { return m_movementDelta.y(); }
		const Point& clientLocation() const { return m_clientLocation; }

		void setLocation(int x, int y) { m_point.SetPoint(x, y); }
		int x() const;
		int y() const;

		Button button() const { return m_button; }
		void setButton(Button b) { m_button = b; }

		int clickCount() const { return m_clickCount; }
		void setClickCount(int i) { m_clickCount = i; }


		virtual bool IsMouseEvent() const override;
		virtual bool IsDragEvent() const override final;
	protected:

		Point m_screenLocation;//ÆÁÄ»×ø±ê
		Point m_clientLocation;//client×ø±ê
		Point m_movementDelta;//
		Point m_point;

		Button m_button;
		int m_clickCount;

	};

}