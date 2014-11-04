#pragma once
#include "base/basictypes.h"
#include <regex>
#include <functional>

namespace ui
{
	class StyleReader
	{
	public:
		struct Mark{
			int cursor;
			int line;
			int col;
		};

		std::string input_;
		int line_;
		int col_;
		int cursor_;
		Mark* bookmark_;

		StyleReader(const std::string& text);

		int getCol();
		int getLine();
		bool eof();

		char peek(int count = 1);

		char read();

		void mark();

		void reset();

		bool readTo(const std::string& pattern, std::string& result);

		std::string readWhile(std::function<bool(char)> filter);

		bool readMatch(const std::string& matcher, std::string& result);

		bool readMatch(const std::regex& matcher, std::string& result);

		std::string readCount(int count);
	};
}