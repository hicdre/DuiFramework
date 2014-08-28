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

	enum EventType{
		EVENT_UNKNOWN = 0,
		EVENT_MOUSE_DOWN,
		EVENT_MOUSE_DBCLICK,
		EVENT_MOUSE_UP,
		EVENT_MOUSE_MOVE,
		EVENT_MOUSE_ENTER,
		EVENT_MOUSE_LEAVE,

		EVENT_KEY_PRESSED,
		EVENT_KEY_RELEASED,
		EVENT_CHAR,

		EVENT_GAIN_FOCUS,
		EVENT_LOSE_FOCUS,

		EVENT_MOUSEWHEEL,

		EVENT_BUTTON_CLICKED,
	};

	enum NoneFlag{
		FLAG_NONE = 0,
	};

	enum KeyFlag{
		KEY_CAPS_LOCK = 1 << 0,
		KEY_SHIFT = 1 << 1,
		KEY_CONTROL = 1 << 2,
		KEY_ALT = 1 << 3,
	};

	enum MouseFlag{
		MOUSE_LEFT = 1 << 4,
		MOUSE_MIDDLE = 1 << 5,
		MOUSE_RIGHT = 1 << 6,
	};

	//enum 
#if 0
	enum LayoutSizePolicy{
		LAYOUT_SIZE_FIXED,
		LAYOUT_SIZE_PREFERED,
		LAYOUT_SIZE_SHRINK,
	};

	const char* k_fluid_orientation = "fluid-orientation";
	const char* k_height_policy = "height-policy";
#endif
}