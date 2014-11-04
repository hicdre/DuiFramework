#pragma once
#include "base/basictypes.h"
#include "style/style_reader.h"
#include "style/style_tokens.h"

namespace ui
{
	struct StyleToken
	{
		int type;
		std::string value;
		int startLine;
		int startCol;
		int endLine;
		int endCol;
	};

	class StyleTokenReader
	{
	public:
		StyleReader* reader_;
		StyleToken* token_;
		StyleTokens tokenData_;
		std::vector<StyleToken*> lt_;
		int ltIndex_;
		std::vector<int> ltIndexCache_;

		StyleTokenReader(const std::string& input);

		bool match(const std::vector<int>& tokenTypes, void* channel);

		void advance(const std::vector<int>& tokenTypes, void* channel);

		int get(void* channel);

		StyleToken* getToken(void* channel);

		StyleToken* createToken(int tt, const std::string& value, int startLine, int startCol);

		StyleToken* commentToken(char ch, int startLine, int startCol);

		StyleToken* charToken(char ch, int startLine, int startCol);

		StyleToken* hashToken(char ch, int startLine, int startCol);

		StyleToken* stringToken(char first, int startLine, int startCol);

		StyleToken* numberToken(char first, int startLine, int startCol);

		StyleToken* whitespaceToken(char first, int startLine, int startCol);

		StyleToken* identOrFunctionToken(char first, int startLine, int startCol);

		std::string readComment(char first);

		std::string readName(char first);

		std::string readEscape(char first);

		std::string readWhitespace();

		std::string readNumber(char first);

		std::string readURI(const std::string& first);

		std::string readURL();

		std::string readString();
	};
}