#pragma once
#include "event/input_event.h"

namespace ui
{
	class View;
	class UIElement;
	class MouseEvent : public InputEvent
	{
	public:
		enum Button {
			ButtonNone = -1,
			ButtonLeft,
			ButtonMiddle,
			ButtonRight
		};
		static MouseEvent* Create();
		static MouseEvent* Create(EventType eventType, MouseEvent* other, int detail, UIElement* relatedTarget);

		void setScreenLocation(int x, int y) { m_screenLocation.SetPoint(x, y); }
		int screenX() const { return m_screenLocation.x(); }
		int screenY() const { return m_screenLocation.y(); }
		const Point& screenLocation() const { return m_screenLocation; }

		void setClientLocation(int x, int y) { m_clientLocation.SetPoint(x, y); }
		int clientX() const { return m_clientLocation.x(); }
		int clientY() const { return m_clientLocation.y(); }
		const Point& clientLocation() const { return m_clientLocation; }

		void setMovementDelta(int x, int y) { m_movementDelta.SetPoint(x, y); }
		int movementX() const { return m_movementDelta.x(); }
		int movementY() const { return m_movementDelta.y(); }
		const Point& movementDelta() const { return m_movementDelta; }
		

		int x();
		int y();

		Button button() const { return m_button; }
		void setButton(Button b) { m_button = b; }

		int clickCount() const { return m_clickCount; }
		void setClickCount(int i) { m_clickCount = i; }

		virtual bool IsMouseEvent() const override;
		virtual bool IsDragEvent() const override final;
		virtual void AdjustForTarget();
	protected:
		MouseEvent();
		MouseEvent(EventType eventType,
			bool canBubble, bool cancelable,
			int modifiers,
			const Point& screenLocation,
			const Point& clientLocation,
			const Point& movementDelta,
			Button button,
			int detail, UIElement* relatedTarget);

		void CalcLocation();

		Point m_screenLocation;//ÆÁÄ»×ø±ê
		Point m_clientLocation;//client×ø±ê
		Point m_movementDelta;//

		bool m_needCalcLocation;
		Point m_point;

		Button m_button;
		int m_clickCount;

		scoped_refptr<UIElement> related_target_;

	};

}