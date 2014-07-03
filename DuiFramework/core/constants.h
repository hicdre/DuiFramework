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
		//TEXT_MUTLILINE = 1 << 5, //多行, 否则单行
		TEXT_NOCLIP = 1 << 6,//不进行裁剪


		TEXT_PATH_ELLIPSIS = 1 << 7,//超出中间用省略号
		TEXT_END_ELLIPSIS = 1 << 8,//超出结尾用省略号
		TEXT_END_WORD_ELLIPSIS = 1 << 9,//超出结尾用省略号
	};
}