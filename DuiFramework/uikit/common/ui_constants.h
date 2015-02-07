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