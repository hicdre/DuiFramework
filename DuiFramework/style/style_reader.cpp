#include "stdafx.h"
#include "style_reader.h"

namespace ui
{


	std::string StyleReader::readCount(int count)
	{
		std::string buffer;
		while (count++)
		{
			char ch = read();
			if (!ch)
				break;
			buffer.push_back(ch);
		}
		return buffer;
	}

	bool StyleReader::readMatch(const std::regex& matcher, std::string& result)
	{
		std::match_results<const char*> regex_result;
		if (std::regex_match(&input_[cursor_], regex_result, matcher))
		{
			regex_result.size();
			assert(0);
		}
		return true;
	}

	bool StyleReader::readMatch(const std::string& matcher, std::string& result)
	{
		if (!strncmp(&input_[cursor_], matcher.c_str(), matcher.size()))
		{
			result = readCount(matcher.size());
			return true;
		}
		return false;
	}

	std::string StyleReader::readWhile(std::function<bool(char)> filter)
	{
		std::string buffer;
		char ch = read();
		while (ch && filter(ch)) {
			buffer.push_back(ch);
			ch = read();
		}
		return buffer;
	}

	bool StyleReader::readTo(const std::string& pattern, std::string& result)
	{
		result.clear();

		while (result.size() < pattern.length()
			|| result.find(pattern) != result.size() - pattern.size())
		{
			char ch = read();
			if (ch)
				result.push_back(ch);
			else
				return false;
		}
		return true;
	}

	void StyleReader::reset()
	{
		if (bookmark_)
		{
			cursor_ = bookmark_->cursor;
			line_ = bookmark_->line;
			col_ = bookmark_->col;
			delete bookmark_;
			bookmark_ = NULL;
		}
	}

	void StyleReader::mark()
	{
		if (!bookmark_)
			bookmark_ = new Mark;
		bookmark_->cursor = cursor_;
		bookmark_->line = line_;
		bookmark_->col = col_;
	}

	char StyleReader::read()
	{
		char ch = 0;
		if (cursor_ < input_.size()) {
			if (input_[cursor_] == '\n') {
				line_++;
				col_ = 1;
			}
			else {
				col_++;
			}
			ch = input_[cursor_++];
		}
		return ch;
	}

	char StyleReader::peek(int count /*= 1*/)
	{
		char ch = 0;
		if (cursor_ < input_.size()) {
			ch = input_[cursor_ + count - 1];
		}
		return ch;
	}

	bool StyleReader::eof()
	{
		return cursor_ == input_.size();
	}

	int StyleReader::getLine()
	{
		return line_;
	}

	int StyleReader::getCol()
	{
		return col_;
	}

	StyleReader::StyleReader(const std::string& text)
	{
		input_ = std::regex_replace(text, std::regex("\n\r?"), "\n");
		line_ = 1;
		col_ = 1;
		cursor_ = 0;
		bookmark_ = NULL;
	}

}