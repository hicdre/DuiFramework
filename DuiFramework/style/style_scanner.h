#pragma once
#include "base/basictypes.h"

namespace ui
{
	enum StyleTokenType {
		// White space of any kind.  No value fields are used.  Note that
		// comments do *not* count as white space; comments separate tokens
		// but are not themselves tokens.
		Token_Whitespace,     //

		// Identifier-like tokens.  mIdent is the text of the identifier.
		// The difference between ID and Hash is: if the text after the #
		// would have been a valid Ident if the # hadn't been there, the
		// scanner produces an ID token.  Otherwise it produces a Hash token.
		// (This distinction is required by css3-selectors.)
		Token_Ident,          // word
		Token_Function,       // word(
		Token_AtKeyword,      // @word
		Token_ID,             // #word
		Token_Hash,           // #0word

		// Numeric tokens.  mNumber is the floating-point value of the
		// number, and mHasSign indicates whether there was an explicit sign
		// (+ or -) in front of the number.  If mIntegerValid is true, the
		// number had the lexical form of an integer, and mInteger is its
		// integer value.  Lexically integer values outside the range of a
		// 32-bit signed number are clamped to the maximum values; mNumber
		// will indicate a 'truer' value in that case.  Percentage tokens
		// are always considered not to be integers, even if their numeric
		// value is integral (100% => mNumber = 1.0).  For Dimension
		// tokens, mIdent holds the text of the unit.
		Token_Number,         // 1 -5 +2e3 3.14159 7.297352e-3
		Token_Dimension,      // 24px 8.5in
		Token_Percentage,     // 85% 1280.4%

		// String-like tokens.  In all cases, mIdent holds the text
		// belonging to the string, and mSymbol holds the delimiter
		// character, which may be ', ", or zero (only for unquoted URLs).
		// Bad_String and Bad_URL tokens are emitted when the closing
		// delimiter or parenthesis was missing.
		Token_String,         // 'foo bar' "foo bar"
		Token_Bad_String,     // 'foo bar
		Token_URL,            // url(foobar) url("foo bar")
		Token_Bad_URL,        // url(foo
		Token_RES,            // res(abc) res("adb")
		Token_Bad_RES,            // res(abc

		// Any one-character symbol.  mSymbol holds the character.
		Token_Symbol,         // . ; { } ! *

		// Match operators.  These are single tokens rather than pairs of
		// Symbol tokens because css3-selectors forbids the presence of
		// comments between the two characters.  No value fields are used;
		// the token type indicates which operator.
		Token_Includes,       // ~=
		Token_Dashmatch,      // |=
		Token_Beginsmatch,    // ^=
		Token_Endsmatch,      // $=
		Token_Containsmatch,  // *=

		// Unicode-range token: currently used only in @font-face.
		// The lexical rule for this token includes several forms that are
		// semantically invalid.  Therefore, mIdent always holds the
		// complete original text of the token (so we can print it
		// accurately in diagnostics), and mIntegerValid is true iff the
		// token is semantically valid.  In that case, mInteger holds the
		// lowest value included in the range, and mInteger2 holds the
		// highest value included in the range.
		Token_URange,         // U+007e U+01?? U+2000-206F

		// HTML comment delimiters, ignored as a unit when they appear at
		// the top level of a style sheet, for compatibility with websites
		// written for compatibility with pre-CSS browsers.  This token type
		// subsumes the css2.1 CDO and CDC tokens, which are always treated
		// the same by the parser.  mIdent holds the text of the token, for
		// diagnostics.
		Token_HTMLComment,    // <!-- -->
	};

	struct StyleToken {
		std::string     mIdent;
		float           mNumber;
		int32			mInteger;
		int32			mInteger2;
		StyleTokenType	mType;
		char	        mSymbol;
		bool            mIntegerValid;
		bool            mHasSign;

		StyleToken()
			: mNumber(0), mInteger(0), mInteger2(0), mType(Token_Whitespace),
			mSymbol('\0'), mIntegerValid(false), mHasSign(false)
		{}

		bool IsSymbol(char aSymbol) const {
			return mType == Token_Symbol && mSymbol == aSymbol;
		}

		void AppendToString(std::string& aBuffer) const;
	};

	class StyleScanner {
	public:
		// |aLineNumber == 1| is the beginning of a file, use |aLineNumber == 0|
		// when the line number is unknown.
		StyleScanner(const std::string& aBuffer, uint32 aLineNumber);
		~StyleScanner();

		// Get the 1-based line number of the last character of
		// the most recently processed token.
		uint32 GetLineNumber() const { return mTokenLineNumber; }

		// Get the 0-based column number of the first character of
		// the most recently processed token.
		uint32 GetColumnNumber() const
		{
			return mTokenOffset - mTokenLineOffset;
		}

		// Get the text of the line containing the first character of
		// the most recently processed token.
		//nsDependentSubstring GetCurrentLine() const;

		// Get the next token.  Return false on EOF.  aTokenResult is filled
		// in with the data for the token.  If aSkipWS is true, skip over
		// eCSSToken_Whitespace tokens rather than returning them.
		bool Next(StyleToken& aTokenResult, bool aSkipWS);

		// Get the body of an URL token (everything after the 'url(').
		// This is exposed for use by nsCSSParser::ParseMozDocumentRule,
		// which, for historical reasons, must make additional function
		// tokens behave like url().  Please do not add new uses to the
		// parser.
		bool NextURL(StyleToken& aTokenResult);

		bool NextRES(StyleToken& aTokenResult);

		// This is exposed for use by nsCSSParser::ParsePseudoClassWithNthPairArg,
		// because "2n-1" is a single DIMENSION token, and "n-1" is a single
		// IDENT token, but the :nth() selector syntax wants to interpret
		// them the same as "2n -1" and "n -1" respectively.  Please do not
		// add new uses to the parser.
		//
		// Note: this function may not be used to back up over a line boundary.
		void Backup(uint32 n);

		// Starts recording the input stream from the current position.
		void StartRecording();

		// Abandons recording of the input stream.
		void StopRecording();

		// Stops recording of the input stream and appends the recorded
		// input to aBuffer.
		void StopRecording(std::string& aBuffer);

	protected:
		int32 Peek(uint32 n = 0);
		void Advance(uint32 n = 1);
		void AdvanceLine();

		void SkipWhitespace();
		void SkipComment();

		bool GatherEscape(std::string& aOutput, bool aInString);
		bool GatherText(uint8 aClass, std::string& aIdent);

		bool ScanIdent(StyleToken& aResult);
		bool ScanAtKeyword(StyleToken& aResult);
		bool ScanHash(StyleToken& aResult);
		bool ScanNumber(StyleToken& aResult);
		bool ScanString(StyleToken& aResult);
		bool ScanURange(StyleToken& aResult);

		const char *mBuffer;
		uint32 mOffset;
		uint32 mCount;

		uint32 mLineNumber;
		uint32 mLineOffset;

		uint32 mTokenLineNumber;
		uint32 mTokenLineOffset;
		uint32 mTokenOffset;

		uint32 mRecordStartOffset;

		//mozilla::css::ErrorReporter *mReporter;

		bool mRecording;
	};
}