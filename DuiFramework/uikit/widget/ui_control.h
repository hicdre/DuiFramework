#pragma once
#include "uikit/widget/ui_view.h"

namespace ui
{
	class UIControl : public UIView
	{
	public:
		typedef uint32 State;
		typedef uint32 Events;

		void sendActionsForControlEvents(Events events);
		void addTargetForControlEvents(UIObject* obj, const Selector& s, Events events);
		void removeTargetForControlEvents(UIObject* obj, const Selector& s, Events events);

		State state() const;

		bool isEnabled() const { return enabled_; }
		void setEnabled(bool v);

		bool isSelected() const { return selected_; }
		void setSelected(bool v);

		bool isHovered() const { return hovered_; }
		void setHovered(bool v);

		bool isPressed() const { return pressed_; }
		void setPressed(bool v);

		UIControlContentVerticalAlignment contentVerticalAlignment() const {
			return contentVerticalAlignment_;
		}
		void setContentVerticalAlignment(UIControlContentVerticalAlignment v);

		UIControlContentHorizontalAlignment contentHorizontalAlignment() const {
			return contentHorizontalAlignment_;
		}
		void setContentHorizontalAlignment(UIControlContentHorizontalAlignment v);

		virtual bool beginTrackingMouse(UIMouse* mouse, UIEvent* event);
		virtual bool continuTrackingMouse(UIMouse* mouse, UIEvent* event);
		virtual void endTrackingMouse(UIMouse* mouse, UIEvent* event);
	protected:
		virtual void OnEnabledChanged();
		virtual void OnSelectedChanged();
		virtual void OnHoveredChanged();
		virtual void OnPressedChanged();
	private:
		bool enabled_{ true };
		bool selected_{ false };
		bool pressed_{ false };
		bool hovered_{ false };

		UIControlContentVerticalAlignment contentVerticalAlignment_{ UIControlContentVerticalAlignmentTop };
		UIControlContentHorizontalAlignment contentHorizontalAlignment_{ UIControlContentHorizontalAlignmentLeft };

		struct ActionTargetItem {
			UIObject* obj;
			Selector sel;
			Events events;
		};
		typedef std::vector<ActionTargetItem> ActionTargetTable;
		ActionTargetTable actionTargetTable_;

	};
}