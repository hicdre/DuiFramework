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

		//TEXT_WORDBREAK = 1 << 4, //按单词换行
		TEXT_MUTLILINE = 1 << 5, //多行, 否则单行
		TEXT_NOCLIP = 1 << 6,//不进行裁剪


		TEXT_PATH_ELLIPSIS = 1 << 7,//超出中间用省略号
		TEXT_END_ELLIPSIS = 1 << 8,//超出结尾用省略号
		TEXT_END_WORD_ELLIPSIS = 1 << 9,//超出结尾用省略号
	};

	enum EventPhaseType{
		EVENT_NONE = 0,
		EVENT_CAPTURING_PHASE,//捕获事件
		EVENT_AT_TARGET,//目标事件
		EVENT_BUBBLING_PHASE,//冒泡事件
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