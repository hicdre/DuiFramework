#pragma once
#include "base/basictypes.h"
#include <unordered_map>

namespace ui
{
	enum StyleTokenName {

		TEOF = 0,

		//HTML-style comments
		CDO,
		CDC,

		//ignorables
		S, 
		COMMENT,

		//identifier types
		STRING,
		IDENT,
		HASH,

		//measurements
		LENGTH,
		ANGLE,
		TIME,
		FREQ,
		DIMENSION,
		PERCENTAGE,
		NUMBER,

		//functions
		URI,
		FUNCTION,

		//Unicode ranges
		UNICODE_RANGE,

		//invalid string
		INVALID,

		//modifier
		NOT,

		//Not defined as tokens, but might as well be
		PIPE,
		SLASH,
		MINUS,
		STAR,
		LBRACE,
		RBRACE,
		LBRACKET,
		RBRACKET,
		EQUALS,
		COLON,
		SEMICOLON,
		LPAREN,
		RPAREN,
		DOT,

	};
	struct StyleTokenItem
	{
		StyleTokenName name;
		std::string text;
		//bool hide{ false };
		char endChar{ 0 };
	};

	class StyleTokens
	{
	public:
		static std::vector<StyleTokenItem*> items_;
		static std::unordered_map<std::string, int> typeMap_;
		StyleTokens()
		{
			AddItem(TEOF);

			AddItem(CDO);
			AddItem(CDC);

			AddItem(S);//whitespace: true
			AddItem(COMMENT);//comment : true, channel: comment, hide : true

			AddItem(STRING);
			AddItem(IDENT);
			AddItem(HASH);

			AddItem(LENGTH);
			AddItem(ANGLE);
			AddItem(TIME);
			AddItem(FREQ);
			AddItem(DIMENSION);
			AddItem(PERCENTAGE);
			AddItem(NUMBER);

			AddItem(URI);
			AddItem(FUNCTION);

			AddItem(UNICODE_RANGE);

			AddItem(INVALID);

			AddItem(NOT);

			AddItem(PIPE, "|");
			AddItem(SLASH, "/");
			AddItem(MINUS, "-");
			AddItem(STAR, "*");
			AddItem(LBRACE, "{", '}');
			AddItem(RBRACE, "}");
			AddItem(LBRACKET, "[", ']');
			AddItem(RBRACKET, "]");
			AddItem(EQUALS, "=");
			AddItem(COLON, ":");
			AddItem(SEMICOLON, ";");
			AddItem(LPAREN, "(", ')');
			AddItem(RPAREN, ")");
			AddItem(DOT, ".");

			for (size_t i = 0; i < items_.size(); i++)
			{
				typeMap_[items_[i]->text] = i;
			}
		}

		void AddItem(StyleTokenName name, const std::string& text, char endchar = 0)
		{
			StyleTokenItem* item = new StyleTokenItem;
			item->name = name;
			item->text = text;
			item->endChar = endchar;
			items_.push_back(item);
		}

		void AddItem(StyleTokenName name)
		{
			StyleTokenItem* item = new StyleTokenItem;
			item->name = name;
			items_.push_back(item);
		}

		static StyleTokenName name(int tt){
			return (StyleTokenName)tt;
		}

		static StyleTokenItem* at(int i){
			return items_[i];
		}

		static int type(const std::string& t) {
			if (typeMap_.count(t))
				return typeMap_[t];
			else
				return -1;
		}

		static int type(char t) {
			return type(std::string(t, 1));
		}

		static int length() {
			return items_.size();
		}
	};
}