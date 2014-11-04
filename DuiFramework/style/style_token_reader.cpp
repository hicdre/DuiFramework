#include "stdafx.h"
#include "style_token_reader.h"

namespace ui
{
	namespace
	{
		std::regex h("^[0-9a-fA-F]$");
		//std::regex nonascii(R"(^[\u0080-\uFFFF]$)");
		//std::regex nl("^[\n|\r\n|\r|\f");

		bool isHexDigit(char ch){
			return ch &&
				(('0' <= ch && ch <= '9')
				|| ('a' <= ch && ch <= 'f')
				|| ('A' <= ch && ch <= 'F'));
		}

		bool isDigit(char ch){
			return ch && ('0' <= ch && ch <= '9');
		}

		bool isWhitespace(char ch){
			return ch && isspace(ch);
		}

		bool isNewLine(char ch){
			return ch && (ch == '\r' || ch == '\n');
		}

		bool isNameStart(char ch){
			return ch && std::regex_match(std::string(1, ch), std::regex(R"([a-z_\x80-\xFF\\])", std::regex_constants::icase));
		}

		bool isNameChar(char ch){
			return ch && (isNameStart(ch) || std::regex_match(std::string(1,ch), std::regex(R"([0-9\-\\])")));
		}

		bool isIdentStart(char ch){
			return ch && (isNameStart(ch) || std::regex_match(std::string(1,ch), std::regex(R"(\-\\)")));
		}
	}

	std::vector<StyleTokenItem*> StyleTokens::items_;
	std::unordered_map<std::string, int> StyleTokens::typeMap_;

		std::string StyleTokenReader::readString()
		{
			char delim = reader_->read();
			char prev = delim;
			std::string str(1,prev);
			char ch = reader_->peek();

			while (ch)
			{
				ch = reader_->read();
				str += ch;

				if (ch == delim && prev != '\\')
					break;

				if (isNewLine(reader_->peek()) && ch != '\\') {
					str.clear();
					break;
				}

				prev = ch;
				ch = reader_->peek();
			}

			if (!ch)
				str.clear();

			return str;
		}

		std::string StyleTokenReader::readURL()
		{
			std::string url;
			char ch = reader_->peek();
			std::regex reg("^[!#$%&\\*-~]$");
			while (std::regex_match(std::string(1,ch), reg))
			{
				url += reader_->read();
				ch = reader_->peek();
			}
			return url;
		}

		std::string StyleTokenReader::readURI(const std::string& first)
		{
			std::string uri = first;
			std::string inner;

			char ch = reader_->peek();

			reader_->mark();

			while (ch && isWhitespace(ch)) {
				reader_->read();
				ch = reader_->peek();
			}

			if (ch == '\'' || ch == '"') {
				inner = readString();
			}
			else {
				inner = readURL();
			}

			ch = reader_->peek();

			while (ch && isWhitespace(ch)) {
				reader_->read();
				ch = reader_->peek();
			}

			if (inner.empty() || ch != ')') {
				uri = first;
				reader_->reset();
			}
			else {
				uri += inner + reader_->read();
			}
			return uri;
		}

		std::string StyleTokenReader::readNumber(char first)
		{
			bool hasDot = (first == '.');
			std::string number(1, first);
			char ch = reader_->peek();

			while (ch)
			{
				if (isDigit(ch))
					number.push_back(reader_->read());
				else if (ch == '.')
				{
					if (hasDot)
						break;
					else
					{
						hasDot = true;
						number.push_back(reader_->read());
					}
				}
				else
					break;
				ch = reader_->peek();
			}
			return number;
		}

		std::string StyleTokenReader::readWhitespace()
		{
			std::string whitespace;
			char ch = reader_->peek();

			while (isWhitespace(ch)){
				reader_->read();
				whitespace += ch;
				ch = reader_->peek();
			}
			return whitespace;
		}

		std::string StyleTokenReader::readEscape(char first)
		{
			std::string cssEscape;
			if (first)
				cssEscape.push_back(first);
			char ch = reader_->peek();
			int i = 0;
			if (isHexDigit(ch)) {
				do {
					cssEscape.push_back(reader_->read());
					ch = reader_->peek();
				} while (ch && isHexDigit(ch) && ++i < 6);
			}

			if (cssEscape.size() == 3 && isspace(ch) ||
				cssEscape.size() == 7 || cssEscape.size() == 1) {
				reader_->read();
				cssEscape.push_back(ch);
			}

			return cssEscape;
		}

		std::string StyleTokenReader::readName(char first)
		{
			std::string ident;
			if (first)
				ident.push_back(first);
			char ch = reader_->peek();
			while (true)
			{
				if (ch == '\\')
				{
					ident.append(readEscape(reader_->read()));
				}
				else if (ch && isNameChar(ch))
				{
					ident.push_back(reader_->read());
					ch = reader_->peek();
				}
				else
				{
					break;
				}
			}
			return ident;
		}

		std::string StyleTokenReader::readComment(char first)
		{
			StyleReader* reader = reader_;
			std::string comment;
			if (first) {
				comment.push_back(first);
			}
			char ch = reader->read();
			if (ch == '*')
			{
				while (ch)
				{
					comment.push_back(ch);

					if (comment.size() > 2 && ch == '*' && reader->peek() == '/') {
						comment.push_back(reader->read());
						break;
					}

					ch = reader->read();
				}
				return comment;
			}
			else
			{
				return "";
			}
		}

		StyleToken* StyleTokenReader::identOrFunctionToken(char first, int startLine, int startCol)
		{
			std::string ident = readName(first);
			int tt = IDENT;

			if (reader_->peek() == '(') {
				ident += reader_->read();
				if (!_strnicmp(ident.c_str(), "url(", 4)) {
					tt = URI;
					ident = readURI(ident);

					if (!_strnicmp(ident.c_str(), "url(", 4)) {
						tt = FUNCTION;
					}
				}
			}

			return createToken(tt, ident, startLine, startCol);
		}

		StyleToken* StyleTokenReader::whitespaceToken(char first, int startLine, int startCol)
		{
			std::string value;
			value += first;
			value += readWhitespace();
			return createToken(S, value, startLine, startCol);
		}

		StyleToken* StyleTokenReader::numberToken(char first, int startLine, int startCol)
		{
			std::string value = readNumber(first);
			int tt = NUMBER;
			std::string ident;
			char ch = reader_->peek();
			if (isIdentStart(ch))
			{
				ident = readName(reader_->read());
				value += ident;
				if (std::regex_match(ident, std::regex("^px$", std::regex_constants::icase)))
				{
					tt = LENGTH;
				}
				else if (std::regex_match(ident, std::regex("^deg|^rad$|^grad$", std::regex_constants::icase)))
				{
					tt = ANGLE;
				}
				else if (std::regex_match(ident, std::regex("^ms$|^s$", std::regex_constants::icase)))
				{
					tt = TIME;
				}
				else if (std::regex_match(ident, std::regex("^hz$|^khz$", std::regex_constants::icase)))
				{
					tt = FREQ;
				}
				else
				{
					tt = DIMENSION;
				}
			}
			else if (ch == '%')
			{
				value.push_back(reader_->read());
				tt = PERCENTAGE;
			}
			return createToken(tt, value, startLine, startCol);
		}

		StyleToken* StyleTokenReader::stringToken(char first, int startLine, int startCol)
		{
			char delim = first;
			char prev = first;
			std::string str(1, first);
			int tt = STRING;
			char ch = reader_->read();
			while (ch)
			{
				str.push_back(ch);

				if (ch == delim && prev != '\\')
					break;

				if (isNewLine(reader_->peek()) && ch != '\\') {
					tt = INVALID;
					break;
				}

				prev = ch;
				ch = reader_->read();
			}

			if (ch == 0) {
				tt = INVALID;
			}

			return createToken(tt, str, startLine, startCol);
		}

		StyleToken* StyleTokenReader::hashToken(char ch, int startLine, int startCol)
		{
			std::string name = readName(ch);
			return createToken(HASH, name, startLine, startCol);
		}

		StyleToken* StyleTokenReader::charToken(char ch, int startLine, int startCol)
		{
			int tt = StyleTokens::type(ch);
			if (tt == -1)
				return NULL;
			return createToken(tt, std::string(1,ch), startLine, startCol);
		}

		StyleToken* StyleTokenReader::commentToken(char ch, int startLine, int startCol)
		{
			std::string comment = readComment(ch);
			return createToken(COMMENT, comment, startLine, startCol);
		}

		StyleToken* StyleTokenReader::createToken(int tt, const std::string& value, int startLine, int startCol)
		{
			StyleToken* token = new StyleToken;
			token->type = tt;
			token->value = value;
			token->startLine = startLine;
			token->startCol = startCol;
			token->endLine = reader_->getLine();
			token->endCol = reader_->getCol();
			return token;
		}

		StyleToken* StyleTokenReader::getToken(void* channel)
		{
			StyleReader* reader = reader_;
			StyleToken* token = NULL;
			int startLine = reader->getLine();
			int startCol = reader->getCol();
			char ch = reader->read();
			while (ch)
			{
				switch (ch)
				{
					/*
					* Potential tokens:
					* - COMMENT
					* - SLASH
					* - CHAR
					*/
				case '/':
					if (reader->peek() == '*')
						token = commentToken(ch, startLine, startCol);
					else
						token = charToken(ch, startLine, startCol);
					break;

					/*
				* Potential tokens:
				* - STRING
				* - INVALID
				*/
				case '\"':
				case '\'':
					token = stringToken(ch, startLine, startCol);
					break;

					/*
					* Potential tokens:
					* - HASH
					* - CHAR
					*/
				case '#':
					if (isNameChar(reader->peek())){
						token = hashToken(ch, startLine, startCol);
					}
					else {
						token = charToken(ch, startLine, startCol);
					}
					break;

					/*
					* Potential tokens:
					* - DOT
					* - NUMBER
					* - DIMENSION
					* - PERCENTAGE
					*/
				case '.':
					if (isDigit(reader_->peek())){
						token = numberToken(ch, startLine, startCol);
					}
					else {
						token = charToken(ch, startLine, startCol);
					}
					break;

				default:
					/*
					* Potential tokens:
					* - NUMBER
					* - DIMENSION
					* - LENGTH
					* - FREQ
					* - TIME
					* - EMS
					* - EXS
					* - ANGLE
					*/
					if (isDigit(ch)){
						token = numberToken(ch, startLine, startCol);
					}
					/*
					* Potential tokens:
					* - S
					*/
					else if (isWhitespace(ch)){
						token = whitespaceToken(ch, startLine, startCol);
					}
					/*
					* Potential tokens:
					* - IDENT
					*/
					else if (isIdentStart(ch)){
						token = identOrFunctionToken(ch, startLine, startCol);
					}
					/*
					* Potential tokens:
					* - CHAR
					* - PLUS
					*/
					else
					{
						token = charToken(ch, startLine, startCol);
					}

				}
				break;
			}

			if (!token && ch == NULL) {
				token = createToken(TEOF, "", startLine, startCol);
			}
			return token;
		}

		int StyleTokenReader::get(void* channel)
		{
			StyleTokens* tokenInfo = &tokenData_;
			StyleReader* reader = reader_;
			int i = 0;
			int len = tokenInfo->length();
			bool found = false;
			StyleTokenItem* info = NULL;
			StyleToken* token = NULL;
			if (lt_.size() && ltIndex_ >= 0 && ltIndex_ < lt_.size())
			{
				i++;
				token_ = lt_[ltIndex_++];
				info = tokenInfo->at(token_->type);

				//obey channels logic
				// 				while ((info->channel != = undefined && channel != = info.channel) &&
				// 					this._ltIndex < this._lt.length){
				// 					this._token = this._lt[this._ltIndex++];
				// 					info = tokenInfo[this._token.type];
				// 					i++;
				// 				}
				// 
				// 				//here be dragons
				// 				if ((info.channel == = undefined || channel == = info.channel) &&
				// 					this._ltIndex <= this._lt.length){
				// 					this._ltIndexCache.push(i);
				// 					return this._token.type;
				// 				}
			}

			token = getToken(NULL);

			//if it should be hidden, don't save a token
			if (token->type > -1 && token->type != COMMENT)
			{
				//no channel

				token_ = token;
				lt_.push_back(token);

				ltIndexCache_.push_back(lt_.size() - ltIndex_ + i);

				if (lt_.size() > 5) {
					lt_.erase(lt_.begin());
				}

				if (ltIndexCache_.size() > 5) {
					ltIndexCache_.erase(ltIndexCache_.begin());
				}

				ltIndex_ = lt_.size();
			}

			/*
			* Skip to the next token if:
			* 1. The token type is marked as hidden.
			* 2. The token type has a channel specified and it isn't the current channel.
			*/
			info = tokenInfo->at(token->type);
			if (info && info->name == COMMENT) {
				return get(NULL);
			}
			return token->type;
		}

		void StyleTokenReader::advance(const std::vector<int>& tokenTypes, void* channel)
		{
			// 			while (LA(0) && !match(tokenTypes, channel)){
			// 				get();
			// 			}
			// 
			// 			LA(0);
			return;
		}

		bool StyleTokenReader::match(const std::vector<int>& tokenTypes, void* channel)
		{
			// 			if(!(tokenTypes instanceof Array)){
			// 				tokenTypes = [tokenTypes];
			// 			}
			// 
			// 			var tt = this.get(channel),
			// 				i = 0,
			// 				len = tokenTypes.length;
			// 
			// 			while (i < len){
			// 				if (tt == tokenTypes[i++]){
			// 					return true;
			// 				}
			// 			}
			// 
			// 			//no match found, put the token back
			// 			this.unget();
			// 			return false;
			return false;
		}

		StyleTokenReader::StyleTokenReader(const std::string& input)
		{
			reader_ = new StyleReader(input);
			token_ = NULL;

			ltIndex_ = 0;
		}

}