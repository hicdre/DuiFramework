#include "stdafx.h"
#include "lex_parser.h"
#include <regex>

namespace ui
{
	bool StringToInt(const char* s, size_t len, int& val)
	{
		char* end = NULL;
		long ret = strtol(s, &end, 10);
		if (ret == 0 && errno == ERANGE)
		{
			return false;
		}
		val = ret;
		return true;
	}

	bool StringToFloat(const char* s, size_t len, float& val)
	{
		char* end = NULL;
		float ret = strtof(s, &end);
		if (ret == 0 && errno == ERANGE)
		{
			return false;
		}
		val = ret;
		return true;
	}

	bool StringToBool(const char* s, size_t len, bool& val)
	{
		if (!strncmp(s, "true", 4)) {
			val = true;
			return true;
		}

		if (!strncmp(s, "false", 5)) {
			val = false;
			return true;
		}
		return false;
	}

	bool StringToPercent(const char* s, size_t len, float& val)
	{
		return false;
	}

	bool StringToColor(const char* s, size_t len, Color& val)
	{
		return false;
	}

	bool StringToLength(const char* s, size_t len, Length& length)
	{
		return false;
	}

	bool StringToImage(const char* s, size_t len, Image*& image)
	{
		return false;
	}

}