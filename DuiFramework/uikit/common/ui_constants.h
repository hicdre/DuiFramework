#pragma once

namespace ui
{

	enum TextStyle{
		TEXT_TOP = 0,
		TEXT_LEFT = 0,
		TEXT_CENTER = 1,
		TEXT_RIGHT = 1 << 1,
		TEXT_VCENTER = 1 << 2,
		TEXT_BOTTOM = 1 << 3,

		//TEXT_WORDBREAK = 1 << 4, //�����ʻ���
		TEXT_MUTLILINE = 1 << 5, //����, ������
		TEXT_NOCLIP = 1 << 6,//�����вü�


		TEXT_PATH_ELLIPSIS = 1 << 7,//�����м���ʡ�Ժ�
		TEXT_END_ELLIPSIS = 1 << 8,//������β��ʡ�Ժ�
		TEXT_END_WORD_ELLIPSIS = 1 << 9,//������β��ʡ�Ժ�
	};

	enum EventPhaseType{
		EVENT_NONE = 0,
		EVENT_CAPTURING_PHASE,//�����¼�
		EVENT_AT_TARGET,//Ŀ���¼�
		EVENT_BUBBLING_PHASE,//ð���¼�
	};

	enum UIEventType{
		EVENT_UNKNOWN = 0,

		EVENT_MOUSE,

		EVENT_CUSTOM,
	};

	enum UIEventSubType{
		EVENT_SUB_NONE = 0,
		EVENT_MOUSE_MOVE,
		EVENT_MOUSE_DOWN,
		EVENT_MOUSE_UP,
	};

	enum CursorId {
		Cursor_Unknow = 0,

		Cursor_Arrow,
		Cursor_Hand,
	};

	enum TextAlignment {
		TextAlignmentLeft = 0,
		TextAlignmentCenter,
		TextAlignmentRight,
	};

	enum VerticalAlignment {
		VerticalAlignmentTop = 0,
		VerticalAlignmentMiddle,
		VerticalAlignmentBottom,
	};

	enum UIControlEvent {
		UIControlEventMouseDown = 1 << 0,
		UIControlEventTouchDownRepeat = 1 << 1,
		UIControlEventTouchDragInside = 1 << 2,
		UIControlEventTouchDragOutside = 1 << 3,
		UIControlEventTouchDragEnter = 1 << 4,
		UIControlEventTouchDragExit = 1 << 5,
		UIControlEventClick = 1 << 6,
		UIControlEventTouchUpOutside = 1 << 7,
		UIControlEventTouchCancel = 1 << 8,

		UIControlEventValueChanged = 1 << 12,

		UIControlEventEditingDidBegin = 1 << 16,
		UIControlEventEditingChanged = 1 << 17,
		UIControlEventEditingDidEnd = 1 << 18,
		UIControlEventEditingDidEndOnExit = 1 << 19,

		UIControlEventAllTouchEvents = 0x00000FFF,
		UIControlEventAllEditingEvents = 0x000F0000,
		UIControlEventApplicationReserved = 0x0F000000,
		UIControlEventSystemReserved = 0xF0000000,
		UIControlEventAllEvents = 0xFFFFFFFF
	};

	enum UIControlContentHorizontalAlignment {
		UIControlContentHorizontalAlignmentCenter = 0,
		UIControlContentHorizontalAlignmentLeft = 1,
		UIControlContentHorizontalAlignmentRight = 2,
		UIControlContentHorizontalAlignmentFill = 3,
	};

	enum UIControlContentVerticalAlignment {
		UIControlContentVerticalAlignmentCenter = 0,
		UIControlContentVerticalAlignmentTop = 1,
		UIControlContentVerticalAlignmentBottom = 2,
		UIControlContentVerticalAlignmentFill = 3,
	};

	enum UIControlState{
		UIControlStateNormal = 0,
		UIControlStateHovered = 1 << 0, //������ؼ�
		UIControlStatePressed = 1 << 1, //��갴��
		UIControlStateDisabled = 1 << 2,
		UIControlStateSelected = 1 << 3,
	};
}