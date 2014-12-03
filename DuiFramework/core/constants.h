#pragma once

namespace ui
{

	enum HorizontalAlignment{
		ALIGN_LEFT = 0, 
		ALIGN_CENTER,  
		ALIGN_RIGHT, 
	};

	enum VerticalAlignment{
		ALIGN_TOP = 0,
		ALIGN_VCENTER,
		ALIGN_BOTTOM,
	};

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

	enum EventType{
		EVENT_UNKNOWN = 0,
		EVENT_MOUSE_DOWN,
		EVENT_MOUSE_UP,
		
		EVENT_MOUSE_ENTER,
		EVENT_MOUSE_LEAVE,

		EVENT_MOUSE_MOVE,
		EVENT_MOUSE_DRAG,

		EVENT_MOUSE_CLICK,
		EVENT_MOUSE_DBCLICK,

		EVENT_KEY_DOWN,
		EVENT_KEY_UP,
		EVENT_KEY_PRESS,

		EVENT_FOCUS,
		EVENT_BLUR,

		EVENT_SELECT,
		EVENT_CHANGE,
		
	};

}