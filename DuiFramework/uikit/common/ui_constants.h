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

}