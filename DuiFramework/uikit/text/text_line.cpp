#include "stdafx.h"
#include "text_line.h"

namespace ui
{


	TextLine::TextLine()
	{

	}

	TextLine::~TextLine()
	{

	}

	bool TextLine::modify(int start, int count, const wchar_t* input, int inputCount)
	{
		if (start < 0 || count < 0 || inputCount < 0 || start + count < text_.size())
			return;
		int newLength = text_.size() - count + inputCount;
		int oldLength = text_.size();
		if (newLength > oldLength)
			text_.resize(newLength);

		int moveIndex = start + count;
		int moveSize = oldLength - moveIndex;
		if (moveSize > 0)
			memmove(&text_[start + inputCount], &text_[moveIndex], moveSize);
		
		if (inputCount > 0)
			memcpy(&text_[start], input, inputCount);

		text_.resize(newLength);
		return true;
	}

	TextLine TextLine::spilt(int index)
	{
		if (index < 0 || index > text_.size())
			return TextLine();
	}

	void TextLine::append(TextLine& line)
	{

	}

}