#include "stdafx.h"
#include "style_scanner.h"

namespace ui
{
	namespace
	{
		static const uint8 IS_HEX_DIGIT = 0x01;
		static const uint8 IS_IDSTART = 0x02;
		static const uint8 IS_IDCHAR = 0x04;
		static const uint8 IS_URL_CHAR = 0x08;
		static const uint8 IS_HSPACE = 0x10;
		static const uint8 IS_VSPACE = 0x20;
		static const uint8 IS_SPACE = IS_HSPACE | IS_VSPACE;
		static const uint8 IS_STRING = 0x40;

#define H    IS_HSPACE
#define V    IS_VSPACE
#define I    IS_IDCHAR
#define J    IS_IDSTART
#define U    IS_URL_CHAR
#define S    IS_STRING
#define X    IS_HEX_DIGIT

#define SH    S|H
#define SU    S|U
#define SUI   S|U|I
#define SUIJ  S|U|I|J
#define SUIX  S|U|I|X
#define SUIJX S|U|I|J|X

		static const uint8 gLexTable[] = {
			// 00    01    02    03    04    05    06    07
			    0,    S,    S,    S,    S,    S,    S,    S,
			// 08   TAB    LF    0B    FF    CR    0E    0F
			    S,   SH,    V,    S,    V,    V,    S,    S,
			// 10    11    12    13    14    15    16    17
			    S,    S,    S,    S,    S,    S,    S,    S,
			// 18    19    1A    1B    1C    1D    1E    1F
			    S,    S,    S,    S,    S,    S,    S,    S,
			//SPC     !     "     #     $     %     &     '
			   SH,   SU,    0,   SU,   SU,   SU,   SU,    0,
			//  (     )     *     +     ,     -     .     /
			    S,    S,   SU,   SU,   SU,  SUI,   SU,   SU,
			//  0     1     2     3     4     5     6     7
			 SUIX, SUIX, SUIX, SUIX, SUIX, SUIX, SUIX, SUIX,
			//  8     9     :     ;     <     =     >     ?
			 SUIX, SUIX,   SU,   SU,   SU,   SU,   SU,   SU,
			//  @     A     B     C     D     E     F     G
			   SU,SUIJX,SUIJX,SUIJX,SUIJX,SUIJX,SUIJX, SUIJ,
			//  H     I     J     K     L     M     N     O
			 SUIJ, SUIJ, SUIJ, SUIJ, SUIJ, SUIJ, SUIJ, SUIJ,
			//  P     Q     R     S     T     U     V     W
			 SUIJ, SUIJ, SUIJ, SUIJ, SUIJ, SUIJ, SUIJ, SUIJ,
			//  X     Y     Z     [     \     ]     ^     _
			 SUIJ, SUIJ, SUIJ,   SU,    J,   SU,   SU, SUIJ,
			//  `     a     b     c     d     e     f     g
			   SU,SUIJX,SUIJX,SUIJX,SUIJX,SUIJX,SUIJX, SUIJ,
			//  h     i     j     k     l     m     n     o
			 SUIJ, SUIJ, SUIJ, SUIJ, SUIJ, SUIJ, SUIJ, SUIJ,
			//  p     q     r     s     t     u     v     w
			 SUIJ, SUIJ, SUIJ, SUIJ, SUIJ, SUIJ, SUIJ, SUIJ,
			//  x     y     z     {     |     }     ~    7F
			 SUIJ, SUIJ, SUIJ,   SU,   SU,   SU,   SU,    S,
		};

#undef I
#undef J
#undef U
#undef S
#undef X
#undef SH
#undef SU
#undef SUI
#undef SUIJ
#undef SUIX
#undef SUIJX

		/**
		* True if 'ch' is in character class 'cls', which should be one of
		* the constants above or some combination of them.  All characters
		* above U+007F are considered to be in 'cls'.  EOF is never in 'cls'.
		*/
		static inline bool IsOpenCharClass(int32 ch, uint8 cls) {
			return ch >= 0 && (ch >= 128 || (gLexTable[ch] & cls) != 0);
		}

		/**
		* True if 'ch' is in character class 'cls', which should be one of
		* the constants above or some combination of them.  No characters
		* above U+007F are considered to be in 'cls'. EOF is never in 'cls'.
		*/
		static inline bool IsClosedCharClass(int32 ch, uint8 cls) {
			return uint32(ch) < 128 && (gLexTable[ch] & cls) != 0;
		}

		/**
		* True if 'ch' is CSS whitespace, i.e. any of the ASCII characters
		* TAB, LF, FF, CR, or SPC.
		*/
		static inline bool IsWhitespace(int32 ch) {
			return IsClosedCharClass(ch, IS_SPACE);
		}

		/**
		* True if 'ch' is horizontal whitespace, i.e. TAB or SPC.
		*/
		static inline bool IsHorzSpace(int32 ch) {
			return IsClosedCharClass(ch, IS_HSPACE);
		}

		/**
		* True if 'ch' is vertical whitespace, i.e. LF, FF, or CR.  Vertical
		* whitespace requires special handling when consumed, see AdvanceLine.
		*/
		static inline bool IsVertSpace(int32 ch) {
			return IsClosedCharClass(ch, IS_VSPACE);
		}

		/**
		* True if 'ch' is a character that can appear in the middle of an
		* identifier.
		*/
		static inline bool IsIdentChar(int32 ch) {
			return IsOpenCharClass(ch, IS_IDCHAR);
		}

		/**
		* True if 'ch' is a character that by itself begins an identifier.
		* (This is a subset of IsIdentChar.)
		*/
		static inline bool IsIdentStart(int32 ch) {
			return IsOpenCharClass(ch, IS_IDSTART);
		}

		/**
		* True if the two-character sequence aFirstChar+aSecondChar begins an
		* identifier.
		*/
		static inline bool StartsIdent(int32 aFirstChar, int32 aSecondChar)
		{
			return IsIdentStart(aFirstChar) ||
				(aFirstChar == '-' && IsIdentStart(aSecondChar));
		}

		/**
		* True if 'ch' is a decimal digit.
		*/
		static inline bool IsDigit(int32 ch) {
			return (ch >= '0') && (ch <= '9');
		}

		/**
		* True if 'ch' is a hexadecimal digit.
		*/
		static inline bool IsHexDigit(int32 ch) {
			return IsClosedCharClass(ch, IS_HEX_DIGIT);
		}

		/**
		* Assuming that 'ch' is a decimal digit, return its numeric value.
		*/
		static inline uint32 DecimalDigitValue(int32 ch)
		{
			return ch - '0';
		}

		/**
		* Assuming that 'ch' is a hexadecimal digit, return its numeric value.
		*/
		static inline uint32 HexDigitValue(int32 ch)
		{
			if (IsDigit(ch)) {
				return DecimalDigitValue(ch);
			}
			else {
				// Note: c&7 just keeps the low three bits which causes
				// upper and lower case alphabetics to both yield their
				// "relative to 10" value for computing the hex value.
				return (ch & 0x7) + 9;
			}
		}

		/**
		* If 'ch' can be the first character of a two-character match operator
		* token, return the token type code for that token, otherwise return
		* eCSSToken_Symbol to indicate that it can't.
		*/
		static inline StyleTokenType MatchOperatorType(int32 ch)
		{
			switch (ch) {
			case '~': return Token_Includes;
			case '|': return Token_Dashmatch;
			case '^': return Token_Beginsmatch;
			case '$': return Token_Endsmatch;
			case '*': return Token_Containsmatch;
			default:  return Token_Symbol;
			}
		}
	}

	StyleScanner::StyleScanner(const std::string& aBuffer, uint32 aLineNumber)
		: mBuffer(aBuffer.data())
		, mOffset(0)
		, mCount(aBuffer.size())
		, mLineNumber(aLineNumber)
		, mLineOffset(0)
		, mTokenLineNumber(aLineNumber)
		, mTokenLineOffset(0)
		, mTokenOffset(0)
		, mRecordStartOffset(0)
		//, mReporter(nullptr)
		, mRecording(false)
	{
		if (!memcmp(mBuffer, "\xef\xbb\xbf", 3))
		{
			mBuffer += 3;
			mCount -= 3;
		}
	}

	StyleScanner::~StyleScanner()
	{

	}

	bool StyleScanner::Next(StyleToken& aToken, bool aSkipWS)
	{
		int32 ch;

		// do this here so we don't have to do it in dozens of other places
		aToken.mIdent.clear();
		aToken.mType = Token_Symbol;

		for (;;) {
			// Consume any number of comments, and possibly also whitespace tokens,
			// in between other tokens.
			mTokenOffset = mOffset;
			mTokenLineOffset = mLineOffset;
			mTokenLineNumber = mLineNumber;

			ch = Peek();
			if (IsWhitespace(ch)) {
				SkipWhitespace();
				if (!aSkipWS) {
					aToken.mType = Token_Whitespace;
					return true;
				}
				continue; // start again at the beginning
			}
			if (ch == '/' && Peek(1) == '*') {
				// FIXME: Editor wants comments to be preserved (bug 60290).
				SkipComment();
				continue; // start again at the beginning
			}
			break;
		}

		// EOF
		if (ch < 0) {
			return false;
		}

		// 'u' could be UNICODE-RANGE or an identifier-family token
		if (ch == 'u' || ch == 'U') {
			int32 c2 = Peek(1);
			int32 c3 = Peek(2);
			if (c2 == '+' && (IsHexDigit(c3) || c3 == '?')) {
				return ScanURange(aToken);
			}
			return ScanIdent(aToken);
		}

		// identifier family
		if (IsIdentStart(ch)) {
			return ScanIdent(aToken);
		}

		// number family
		if (IsDigit(ch)) {
			return ScanNumber(aToken);
		}

		if (ch == '.' && IsDigit(Peek(1))) {
			return ScanNumber(aToken);
		}

		if (ch == '+') {
			int32 c2 = Peek(1);
			if (IsDigit(c2) || (c2 == '.' && IsDigit(Peek(2)))) {
				return ScanNumber(aToken);
			}
		}

		// '-' can start an identifier-family token, a number-family token,
		// or an HTML-comment
		if (ch == '-') {
			int32 c2 = Peek(1);
			int32 c3 = Peek(2);
			if (IsIdentStart(c2)) {
				return ScanIdent(aToken);
			}
			if (IsDigit(c2) || (c2 == '.' && IsDigit(c3))) {
				return ScanNumber(aToken);
			}
			if (c2 == '-' && c3 == '>') {
				Advance(3);
				aToken.mType = Token_HTMLComment;
				aToken.mIdent.append("-->");
				return true;
			}
		}

		// the other HTML-comment token
		if (ch == '<' && Peek(1) == '!' && Peek(2) == '-' && Peek(3) == '-') {
			Advance(4);
			aToken.mType = Token_HTMLComment;
			aToken.mIdent.append("<!--");
			return true;
		}

		// AT_KEYWORD
		if (ch == '@') {
			return ScanAtKeyword(aToken);
		}

		// HASH
		if (ch == '#') {
			return ScanHash(aToken);
		}

		// STRING
		if (ch == '"' || ch == '\'') {
			return ScanString(aToken);
		}

		// Match operators: ~= |= ^= $= *=
		StyleTokenType opType = MatchOperatorType(ch);
		if (opType != Token_Symbol && Peek(1) == '=') {
			aToken.mType = opType;
			Advance(2);
			return true;
		}

		// Otherwise, a symbol (DELIM).
		aToken.mSymbol = ch;
		Advance();
		return true;
	}

	bool StyleScanner::NextURL(StyleToken& aToken)
	{
		SkipWhitespace();

		int32 ch = Peek();
		if (ch < 0) {
			return false;
		}

		// aToken.mIdent may be "url" at this point; clear that out
		aToken.mIdent.clear();

		// Do we have a string?
		if (ch == '"' || ch == '\'') {
			ScanString(aToken);
			if (aToken.mType == Token_Bad_String) {
				aToken.mType = Token_Bad_URL;
				return true;
			}
			//DCHECK(aToken.mType == Token_String) << "unexpected token type";

		}
		else {
			// Otherwise, this is the start of a non-quoted url (which may be empty).
			aToken.mSymbol = 0;
			GatherText(IS_URL_CHAR, aToken.mIdent);
		}

		// Consume trailing whitespace and then look for a close parenthesis.
		SkipWhitespace();
		ch = Peek();
		if (ch < 0 || ch == ')') {
			Advance();
			aToken.mType = Token_URL;
		}
		else {
			aToken.mType = Token_Bad_URL;
		}
		return true;
	}


	bool StyleScanner::NextRES(StyleToken& aToken)
	{
		SkipWhitespace();

		int32 ch = Peek();
		if (ch < 0) {
			return false;
		}

		// aToken.mIdent may be "url" at this point; clear that out
		aToken.mIdent.clear();

		// Do we have a string?
		if (ch == '"' || ch == '\'') {
			ScanString(aToken);
			if (aToken.mType == Token_Bad_String) {
				aToken.mType = Token_Bad_RES;
				return true;
			}
			//DCHECK(aToken.mType == Token_String) << "unexpected token type";

		}
		else {
			// Otherwise, this is the start of a non-quoted url (which may be empty).
			aToken.mSymbol = 0;
			GatherText(IS_IDCHAR, aToken.mIdent);
		}

		// Consume trailing whitespace and then look for a close parenthesis.
		SkipWhitespace();
		ch = Peek();
		if (ch < 0 || ch == ')') {
			Advance();
			aToken.mType = Token_RES;
		}
		else {
			aToken.mType = Token_Bad_RES;
		}
		return true;
	}



	/**
	* Back up |mOffset| over |n| code units.  Backup(0) is a no-op.
	* If |n| is greater than the distance to beginning of input, will
	* silently stop at the beginning.  May not be used to back up over a
	* line boundary.
	*/
	void StyleScanner::Backup(uint32 n)
	{
		if (mOffset < n)
			mOffset = 0;
		else
			mOffset -= n;
	}

	void StyleScanner::StartRecording()
	{
		//DCHECK(!mRecording) << "already started recording";
		mRecording = true;
		mRecordStartOffset = mOffset;
	}

	void StyleScanner::StopRecording()
	{
		//DCHECK(mRecording) << "haven't started recording";
		mRecording = false;
	}

	void StyleScanner::StopRecording(std::string& aBuffer)
	{
		//DCHECK(mRecording) << "haven't started recording";
		mRecording = false;
		aBuffer.append(mBuffer + mRecordStartOffset,
			mOffset - mRecordStartOffset);
	}

	int32 StyleScanner::Peek(uint32 n /*= 0*/)
	{
		if (mOffset + n >= mCount) {
			return -1;
		}
		return mBuffer[mOffset + n];
	}

	void StyleScanner::Advance(uint32 n /*= 1*/)
	{
		if (mOffset + n >= mCount || mOffset + n < mOffset)
			mOffset = mCount;
		else
			mOffset += n;
	}

	void StyleScanner::AdvanceLine()
	{
		if (mBuffer[mOffset] == '\r' && mOffset + 1 < mCount &&
			mBuffer[mOffset + 1] == '\n')
			mOffset += 2;
		else
			mOffset += 1;
		// 0 is a magical line number meaning that we don't know (i.e., script)
		if (mLineNumber != 0)
			mLineNumber++;
		mLineOffset = mOffset;
	}

	/**
	* Skip over a sequence of whitespace characters (vertical or
	* horizontal) starting at the current read position.
	*/
	void StyleScanner::SkipWhitespace()
	{
		for (;;) {
			int32 ch = Peek();
			if (!IsWhitespace(ch)) { // EOF counts as non-whitespace
				break;
			}
			if (IsVertSpace(ch)) {
				AdvanceLine();
			}
			else {
				Advance();
			}
		}
	}

	void StyleScanner::SkipComment()
	{
		Advance(2);
		for (;;) {
			int32 ch = Peek();
			if (ch < 0) {
				//mReporter->ReportUnexpectedEOF("PECommentEOF");
				//NOTREACHED() << "UnexpectedEOF";
				return;
			}
			if (ch == '*' && Peek(1) == '/') {
				Advance(2);
				return;
			}
			if (IsVertSpace(ch)) {
				AdvanceLine();
			}
			else {
				Advance();
			}
		}
	}

	/**
	* If there is a valid escape sequence starting at the current read
	* position, consume it, decode it, append the result to |aOutput|,
	* and return true.  Otherwise, consume nothing, leave |aOutput|
	* unmodified, and return false.  If |aInString| is true, accept the
	* additional form of escape sequence allowed within string-like tokens.
	*/
	bool StyleScanner::GatherEscape(std::string& aOutput, bool aInString)
	{
		int32 ch = Peek(1);
		if (ch < 0) {
			// Backslash followed by EOF is not an escape.
			return false;
		}
		if (IsVertSpace(ch)) {
			if (aInString) {
				// In strings (and in url() containing a string), escaped
				// newlines are completely removed, to allow splitting over
				// multiple lines.
				Advance();
				AdvanceLine();
				return true;
			}
			// Outside of strings, backslash followed by a newline is not an escape.
			return false;
		}

		if (!IsHexDigit(ch)) {
			// "Any character (except a hexadecimal digit, linefeed, carriage
			// return, or form feed) can be escaped with a backslash to remove
			// its special meaning." -- CSS2.1 section 4.1.3
			Advance(2);
			aOutput.append(1, ch);
			return true;
		}

		return false;
	}

	bool StyleScanner::GatherText(uint8 aClass, std::string& aText)
	{
		uint32 start = mOffset;
		bool inString = aClass == IS_STRING;

		for (;;) {
			// Consume runs of unescaped characters in one go.
			uint32 n = mOffset;
			while (n < mCount && IsOpenCharClass(mBuffer[n], aClass)) {
				n++;
			}
			if (n > mOffset) {
				aText.append(&mBuffer[mOffset], n - mOffset);
				mOffset = n;
			}
			if (n == mCount) {
				break;
			}

			int32 ch = Peek();
			//DCHECK(!IsOpenCharClass(ch, aClass))
			//	<< "should not have exited the inner loop";

			if (ch != '\\') {
				break;
			}
			if (!GatherEscape(aText, inString)) {
				break;
			}
		}

		return mOffset > start;
	}

	bool StyleScanner::ScanIdent(StyleToken& aToken)
	{
		if (!GatherText(IS_IDCHAR, aToken.mIdent)) {
			aToken.mSymbol = Peek();
			Advance();
			return true;
		}

		if (Peek() != '(') {
			aToken.mType = Token_Ident;
			return true;
		}

		Advance();
		aToken.mType = Token_Function;
		//if (aToken.mIdent.LowerCaseEqualsLiteral("url")) {
		if (!_strnicmp(aToken.mIdent.c_str(), "url", 3)){
			NextURL(aToken);
		}
		else if (!_strnicmp(aToken.mIdent.c_str(), "res", 3)) {
			NextRES(aToken);
		}
		return true;
	}

	/**
	* Scan an AtKeyword token.  Also handles production of Symbol when
	* an '@' is not followed by an identifier.
	*/
	bool StyleScanner::ScanAtKeyword(StyleToken& aToken)
	{
		// Fall back for when '@' isn't followed by an identifier.
		aToken.mSymbol = '@';
		Advance();

		int32 ch = Peek();
		if (StartsIdent(ch, Peek(1))) {
			if (GatherText(IS_IDCHAR, aToken.mIdent)) {
				aToken.mType = Token_AtKeyword;
			}
		}
		return true;
	}

	/**
	* Scan a Hash token.  Handles the distinction between eCSSToken_ID
	* and eCSSToken_Hash, and handles production of Symbol when a '#'
	* is not followed by identifier characters.
	*/
	bool StyleScanner::ScanHash(StyleToken& aToken)
	{
		// Fall back for when '#' isn't followed by identifier characters.
		aToken.mSymbol = '#';
		Advance();

		int32 ch = Peek();
		if (IsIdentChar(ch) || ch == '\\') {
			StyleTokenType type =
				StartsIdent(ch, Peek(1)) ? Token_ID : Token_Hash;
			aToken.mIdent.clear();
			if (GatherText(IS_IDCHAR, aToken.mIdent)) {
				aToken.mType = type;
			}
		}

		return true;
	}

	bool StyleScanner::ScanNumber(StyleToken& aToken)
	{
		int32 c = Peek();
		// Sign of the mantissa (-1 or 1).
		int32 sign = c == '-' ? -1 : 1;
		// Absolute value of the integer part of the mantissa.  This is a double so
		// we don't run into overflow issues for consumers that only care about our
		// floating-point value while still being able to express the full int32_t
		// range for consumers who want integers.
		double intPart = 0;
		// Fractional part of the mantissa.  This is a double so that when we convert
		// to float at the end we'll end up rounding to nearest float instead of
		// truncating down (as we would if fracPart were a float and we just
		// effectively lost the last several digits).
		double fracPart = 0;
		// Absolute value of the power of 10 that we should multiply by (only
		// relevant for numbers in scientific notation).  Has to be a signed integer,
		// because multiplication of signed by unsigned converts the unsigned to
		// signed, so if we plan to actually multiply by expSign...
		int32 exponent = 0;
		// Sign of the exponent.
		int32 expSign = 1;

		aToken.mHasSign = (c == '+' || c == '-');
		if (aToken.mHasSign) {
			Advance();
			c = Peek();
		}

		bool gotDot = (c == '.');

		if (!gotDot) {
			// Scan the integer part of the mantissa.
			//DCHECK(IsDigit(c)) << "should have been excluded by logic above";
			do {
				intPart = 10 * intPart + DecimalDigitValue(c);
				Advance();
				c = Peek();
			} while (IsDigit(c));

			gotDot = (c == '.') && IsDigit(Peek(1));
		}

		if (gotDot) {
			// Scan the fractional part of the mantissa.
			Advance();
			c = Peek();
			//DCHECK(IsDigit(c)) << "should have been excluded by logic above";
			// Power of ten by which we need to divide our next digit
			double divisor = 10;
			do {
				fracPart += DecimalDigitValue(c) / divisor;
				divisor *= 10;
				Advance();
				c = Peek();
			} while (IsDigit(c));
		}

		bool gotE = false;
// 		if (IsSVGMode() && (c == 'e' || c == 'E')) {
// 			int32 expSignChar = Peek(1);
// 			int32 nextChar = Peek(2);
// 			if (IsDigit(expSignChar) ||
// 				((expSignChar == '-' || expSignChar == '+') && IsDigit(nextChar))) {
// 				gotE = true;
// 				if (expSignChar == '-') {
// 					expSign = -1;
// 				}
// 				Advance(); // consumes the E
// 				if (expSignChar == '-' || expSignChar == '+') {
// 					Advance();
// 					c = nextChar;
// 				}
// 				else {
// 					c = expSignChar;
// 				}
// 				DCHECK(IsDigit(c)) << "should have been excluded by logic above";
// 				do {
// 					exponent = 10 * exponent + DecimalDigitValue(c);
// 					Advance();
// 					c = Peek();
// 				} while (IsDigit(c));
// 			}
// 		}

		StyleTokenType type = Token_Number;

		// Set mIntegerValid for all cases (except %, below) because we need
		// it for the "2n" in :nth-child(2n).
		aToken.mIntegerValid = false;

		// Time to reassemble our number.
		// Do all the math in double precision so it's truncated only once.
		double value = sign * (intPart + fracPart);
		if (gotE) {
			// Explicitly cast expSign*exponent to double to avoid issues with
			// overloaded pow() on Windows.
			value *= pow(10.0, double(expSign * exponent));
		}
		else if (!gotDot) {
			// Clamp values outside of integer range.
			if (sign > 0) {
				aToken.mInteger = int32((std::min)(intPart, double(kint32max)));
			}
			else {
				aToken.mInteger = int32((std::max)(-intPart, double(kint32min)));
			}
			aToken.mIntegerValid = true;
		}

		std::string& ident = aToken.mIdent;

		// Check for Dimension and Percentage tokens.
		if (c >= 0) {
			if (StartsIdent(c, Peek(1))) {
				if (GatherText(IS_IDCHAR, ident)) {
					type = Token_Dimension;
				}
			}
			else if (c == '%') {
				Advance();
				type = Token_Percentage;
				value = value / 100.0f;
				aToken.mIntegerValid = false;
			}
		}
		aToken.mNumber = value;
		aToken.mType = type;
		return true;
	}

	/**
	* Scan a string constant ('foo' or "foo").  Will always produce
	* either a String or a Bad_String token; the latter occurs when the
	* close quote is missing.  Always returns true (for convenience in Next()).
	*/
	bool StyleScanner::ScanString(StyleToken& aToken)
	{
		int32 aStop = Peek();
		//DCHECK(aStop == '"' || aStop == '\'') << "should not have been called";
		aToken.mType = Token_String;
		aToken.mSymbol = char(aStop); // Remember how it's quoted.
		Advance();

		for (;;) {
			GatherText(IS_STRING, aToken.mIdent);

			int32 ch = Peek();
			if (ch == -1) {
				break; // EOF ends a string token with no error.
			}
			if (ch == aStop) {
				Advance();
				break;
			}
			// Both " and ' are excluded from IS_STRING.
			if (ch == '"' || ch == '\'') {
				aToken.mIdent.append(1, ch);
				Advance();
				continue;
			}

			aToken.mType = Token_Bad_String;
			//mReporter->ReportUnexpected("SEUnterminatedString", aToken);
			break;
		}
		return true;
	}

	/**
	* Scan a unicode-range token.  These match the regular expression
	*
	*     u\+[0-9a-f?]{1,6}(-[0-9a-f]{1,6})?
	*
	* However, some such tokens are "invalid".  There are three valid forms:
	*
	*     u+[0-9a-f]{x}              1 <= x <= 6
	*     u+[0-9a-f]{x}\?{y}         1 <= x+y <= 6
	*     u+[0-9a-f]{x}-[0-9a-f]{y}  1 <= x <= 6, 1 <= y <= 6
	*
	* All unicode-range tokens have their text recorded in mIdent; valid ones
	* are also decoded into mInteger and mInteger2, and mIntegerValid is set.
	* Note that this does not validate the numeric range, only the syntactic
	* form.
	*/
	bool StyleScanner::ScanURange(StyleToken& aResult)
	{
		int32 intro1 = Peek();
		int32 intro2 = Peek(1);
		int32 ch = Peek(2);

		//DCHECK((intro1 == 'u' || intro1 == 'U') &&
		//	intro2 == '+' &&
		//	(IsHexDigit(ch) || ch == '?'))
		//	<< "should not have been called";

		aResult.mIdent.append(1, intro1);
		aResult.mIdent.append(1, intro2);
		Advance(2);

		bool valid = true;
		bool haveQues = false;
		uint32 low = 0;
		uint32 high = 0;
		int i = 0;

		do {
			aResult.mIdent.append(1, ch);
			if (IsHexDigit(ch)) {
				if (haveQues) {
					valid = false; // All question marks should be at the end.
				}
				low = low * 16 + HexDigitValue(ch);
				high = high * 16 + HexDigitValue(ch);
			}
			else {
				haveQues = true;
				low = low * 16 + 0x0;
				high = high * 16 + 0xF;
			}

			i++;
			Advance();
			ch = Peek();
		} while (i < 6 && (IsHexDigit(ch) || ch == '?'));

		if (ch == '-' && IsHexDigit(Peek(1))) {
			if (haveQues) {
				valid = false;
			}

			aResult.mIdent.append(1, ch);
			Advance();
			ch = Peek();
			high = 0;
			i = 0;
			do {
				aResult.mIdent.append(1, ch);
				high = high * 16 + HexDigitValue(ch);

				i++;
				Advance();
				ch = Peek();
			} while (i < 6 && IsHexDigit(ch));
		}

		aResult.mInteger = low;
		aResult.mInteger2 = high;
		aResult.mIntegerValid = valid;
		aResult.mType = Token_URange;
		return true;
	}
}