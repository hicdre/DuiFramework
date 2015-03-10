#pragma once
#include <string>
namespace ui
{
	class TextLine
	{
	public:
		TextLine();
		~TextLine();

		bool modify(int start, int count, const wchar_t* input, int inputCount);
		TextLine spilt(int index);
		void append(TextLine& line);
	private:
		std::wstring text_;
	};
}