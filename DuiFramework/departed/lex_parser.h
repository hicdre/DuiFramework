#pragma once
#include "base/basictypes.h"
#include "render/image.h"

namespace ui
{
	bool StringToInt(const char* s, size_t len, int& val);
	bool StringToFloat(const char* s, size_t len, float& val);

	// true
	// false
	bool StringToBool(const char* s, size_t len, bool& val);

	// xx%
	bool StringToPercent(const char* s, size_t len, float& val);

	// #FFFFFFFF => 0xFFFFFFFF
	// #ABC => 0xAABBCC
	// rgb(20,20,20) => 0xFF141414
	// rgba(20,20,20,16) =>0x10141414
	bool StringToColor(const char* s, size_t len, Color& val);

	// 23 => fixed int
	// 23.6 => fixed float
	// 23.6% => percent float
	bool StringToLength(const char* s, size_t len, Length& length);

	// id(ID_TEST)
	// local-path("C:/t.png")
	// linear-gradient
	bool StringToImage(const char* s, size_t len, Image*& image);
}