#include "stdafx.h"
#include "style_parser.h"
#include "style_scanner.h"
#include <stack>

namespace ui
{
	namespace
	{
		static void AssignRuleToPointer(StyleRule* aRule, void* aPointer)
		{
			StyleRule **pointer = static_cast<StyleRule**>(aPointer);
			*pointer = aRule;
			aRule->AddRef();
		}
		static void AppendRuleToSheet(StyleRule* aRule, void* aParser)
		{
			StyleParser* parser = (StyleParser*)aParser;
			parser->AppendRule(aRule);
		}
	}


	StyleParser::StyleParser()
	{
		scanner_ = NULL;
		have_bush_back_ = false;
	}

	bool StyleParser::ParseRule(const std::string& buffer, StyleRule** rule)
	{
		*rule = NULL;

		StyleScanner scanner(buffer, 0);
		InitScanner(scanner);

		StyleToken* tk = &token_;
		bool is_ok = true;
		// Get first non-whitespace token
		if (!GetToken(true)) {
			//REPORT_UNEXPECTED(PEParseRuleWSOnly);
			//OUTPUT_ERROR();
			//rv = NS_ERROR_DOM_SYNTAX_ERR;
			is_ok = false;
		}
		else 
		{
			if (Token_AtKeyword == tk->mType) 
			{
				// FIXME: perhaps aInsideBlock should be true when we are?
				//ParseAtRule(AssignRuleToPointer, aResult, false);
			}
			else
			{
				UngetToken();
				ParseRuleSet(AssignRuleToPointer, rule);
			}

			if (*rule && GetToken(true)) {
				// garbage after rule
				//REPORT_UNEXPECTED_TOKEN(PERuleTrailing);
				//NS_RELEASE(*aResult);
				(*rule)->Release();
				*rule = NULL;
			}

			if (!*rule) {
				//rv = NS_ERROR_DOM_SYNTAX_ERR;
				//OUTPUT_ERROR();
				is_ok = false;
			}
		}

		ReleaseScanner();
		return is_ok;
	}


	bool StyleParser::ParseRuleSet(RuleAppendFunc aAppendFunc, void* aData, bool aInsideBraces /*= false*/)
	{
		// First get the list of selectors for the rule
		StyleSelectorList* slist = nullptr;
		uint32 linenum = scanner_->GetLineNumber();
		if (!ParseSelectorList(slist, '{')) {
			//REPORT_UNEXPECTED(PEBadSelectorRSIgnored);
			//OUTPUT_ERROR();
			SkipRuleSet(aInsideBraces);
			return false;
		}
		//NS_ASSERTION(nullptr != slist, "null selector list");
		//CLEAR_ERROR();
		// Next parse the declaration block
		//uint32 parseFlags = eParseDeclaration_InBraces |
		//	eParseDeclaration_AllowImportant;
		StyleDeclarationList* declarations = NULL;
		if (!ParseDeclarationBlock(declarations, true)) {
			delete slist;
			return false;
		}

		// Translate the selector list and declaration block into style data

		scoped_refptr<StyleRule> rule(new StyleRule(slist, declarations));
		rule->SetLineNumber(linenum);
		(*aAppendFunc)(rule.get(), aData);
		return true;
	}


	bool StyleParser::ParseSelectorList(StyleSelectorList*& aListHead, char aStopChar)
	{
		aListHead = NULL;

		scoped_ptr<StyleSelectorList> list(new StyleSelectorList);
		StyleSelector* selector = NULL;
		if (!ParseSelectorGroup(selector)) {
			// must have at least one selector group
			aListHead = NULL;
			return false;
		}
		//NS_ASSERTION(nullptr != list, "no selector list");
		list->Add(selector);
		aListHead = list.get();

		// After that there must either be a "," or a "{" (the latter if
		// StopChar is nonzero)
		StyleToken* tk = &token_;
		for (;;) {
			if (!GetToken(true)) {
				if (aStopChar == 0) {
					list.release();
					return true;
				}

				//REPORT_UNEXPECTED_EOF(PESelectorListExtraEOF);
				break;
			}

			if (Token_Symbol == tk->mType) {
				if (',' == tk->mSymbol) {
					StyleSelector* newList = nullptr;
					// Another selector group must follow
					if (!ParseSelectorGroup(newList)) {
						break;
					}
					// add new list to the end of the selector list
					list->Add(newList);
					continue;
				}
				else if (aStopChar == tk->mSymbol && aStopChar != 0) {
					UngetToken();
					list.release();
					return true;
				}
			}
			//REPORT_UNEXPECTED_TOKEN(PESelectorListExtra);
			UngetToken();
			break;
		}

		aListHead = NULL;
		return false;
	}


	bool StyleParser::ParseSelectorGroup(StyleSelector*& aList)
	{
		char combinator = 0;
		scoped_ptr<StyleSelector> list;

		for (;;) {
			StyleSelector* selector = NULL;
			if (!ParseSelector(selector, combinator)) {
				return false;
			}

			if (list.get()) {
				list->AddChildSelector(selector);
			}
			else {
				list.reset(selector);
			}

			// Look for a combinator.
			if (!GetToken(false)) {
				break; // EOF ok here
			}

			combinator = 0;
			if (token_.mType == Token_Whitespace) {
				if (!GetToken(true)) {
					break; // EOF ok here
				}
				combinator =' ';
			}

			if (token_.mType != Token_Symbol) {
				UngetToken(); // not a combinator
			}
			else {
				char symbol = token_.mSymbol;
				if (symbol == '+' || symbol == '>' || symbol == '~') {
					combinator = token_.mSymbol;
				}
				else {
					UngetToken(); // not a combinator
					if (symbol == ',' || symbol == '{' || symbol == ')') {
						break; // end of selector group
					}
				}
			}

			if (!combinator) {
				//REPORT_UNEXPECTED_TOKEN(PESelectorListExtra);
				return false;
			}
		}

		aList = list.release();
		return true;
	}

	// id | class 
	bool StyleParser::ParseSelector(StyleSelector*& aList, char aPrevCombinator)
	{
		aList = NULL;

		if (!GetToken(true)) {
			//REPORT_UNEXPECTED_EOF(PESelectorEOF);
			return false;
		}
		
		StyleSelector* selector = new StyleSelector;
		bool success = true;
		bool has_id_or_tag = false;
		while (success)
		{
			if (token_.mType == Token_ID)
			{
				if (has_id_or_tag)
					success = false;
				else
					has_id_or_tag = true;
				success = ParseIDSelector(selector);
			}
			else if (token_.mType == Token_Ident) {    // tag
				if (has_id_or_tag)
					success = false;
				else
					has_id_or_tag = true;

				selector->SetTag(token_.mIdent);

				success = true;
			}
			else if (token_.IsSymbol('.')) {    // .class
				success = ParseClassSelector(selector);
			}
			else if (token_.IsSymbol(':')) {    // :pseudo
				success = ParsePseudoSelector(selector);
			}
			else {  // not a selector token, we're done
				UngetToken();
				break;
			}

			if (!success) {
				delete selector;
				selector = NULL;
				break;
			}

			if (!GetToken(false)) { // premature eof is ok (here!)
				break;
			}
		}

		if (success) {
			aList = selector;
			return true;
		}
		return false;
	}


	bool StyleParser::GetToken(bool aSkipWS)
	{
		if (have_bush_back_) {
			have_bush_back_ = false;
			if (!aSkipWS || token_.mType != Token_Whitespace) {
				return true;
			}
		}
		return scanner_->Next(token_, aSkipWS);
	}

	void StyleParser::UngetToken()
	{
		have_bush_back_ = true;
	}

	void StyleParser::InitScanner(StyleScanner& scanner)
	{
		scanner_ = &scanner;
		have_bush_back_ = false;
	}

	void StyleParser::ReleaseScanner()
	{
		scanner_ = NULL;
	}

	void StyleParser::AppendRule(StyleRule* rule)
	{
// 		uint32_t count = mGroupStack.Length();
// 		if (0 < count) {
// 			mGroupStack[count - 1]->AppendStyleRule(aRule);
// 		}
// 		else {
// 			mSheet->AppendStyleRule(aRule);
// 		}
	}

	bool StyleParser::ParseIDSelector(StyleSelector* s)
	{
		s->SetId(token_.mIdent);
		return true;
	}

	bool StyleParser::ParseClassSelector(StyleSelector* s)
	{
		if (!GetToken(false)) { // get ident
			//REPORT_UNEXPECTED_EOF(PEClassSelEOF);
			return false;
		}
		if (Token_Ident != token_.mType) {  // malformed selector
			//REPORT_UNEXPECTED_TOKEN(PEClassSelNotIdent);
			UngetToken();
			return false;
		}

		s->AddClass(token_.mIdent);
		return true;
	}

	bool StyleParser::ParsePseudoSelector(StyleSelector* s)
	{
		if (!GetToken(false)) { // get ident
			//REPORT_UNEXPECTED_EOF(PEClassSelEOF);
			return false;
		}

		if (Token_Ident != token_.mType) {  // malformed selector
			UngetToken();
			return false;
		}

		if (token_.mIdent == "hover") {
			s->SetPseduo(PSEUDO_HOVER);
			return true;
		}
		if (token_.mIdent == "active") {
			s->SetPseduo(PSEUDO_ACTIVE);
			return true;
		}
		if (token_.mIdent == "focus") {
			s->SetPseduo(PSEUDO_FOCUS);
			return true;
		}
		return false;
	}


	bool StyleParser::ParseDeclarationBlock(StyleDeclarationList*& aList, bool inbraces)
	{
		aList = NULL;
		bool checkForBraces = inbraces;
		if (checkForBraces) {
			if (!ExpectSymbol('{', true)) {
				//REPORT_UNEXPECTED_TOKEN(PEBadDeclBlockStart);
				//OUTPUT_ERROR();
				return false;
			}
		}

		StyleDeclarationList* declaration = new StyleDeclarationList;
		//mData.AssertInitialState();
		if (declaration) {
			for (;;) {
				bool changed;
				if (!ParseDeclaration(declaration, inbraces, true, &changed)) {
					if (!SkipDeclaration(checkForBraces)) {
						break;
					}
					if (checkForBraces) {
						if (ExpectSymbol('}', true)) {
							break;
						}
					}
					// Since the skipped declaration didn't end the block we parse
					// the next declaration.
				}
			}
			//declaration->CompressFrom(&mData);
		}
		aList = declaration;
		return true;
	}

	bool StyleParser::ParseDeclaration(StyleDeclarationList* aList, bool inbraces, bool aMustCallValueAppended, bool* aChanged)
	{
		bool checkForBraces = inbraces;

		// Get property name
		StyleToken* tk = &token_;
		std::string propertyName;
		for (;;) {
			if (!GetToken(true)) {
				if (checkForBraces) {
					//REPORT_UNEXPECTED_EOF(PEDeclEndEOF);
				}
				return false;
			}
			if (Token_Ident == tk->mType) {
				propertyName = tk->mIdent;
				// grab the ident before the ExpectSymbol trashes the token
				if (!ExpectSymbol(':', true)) {
					//REPORT_UNEXPECTED_TOKEN(PEParseDeclarationNoColon);
					//REPORT_UNEXPECTED(PEDeclDropped);
					//OUTPUT_ERROR();
					return false;
				}
				break;
			}
			if (tk->IsSymbol(';')) {
				// dangling semicolons are skipped
				continue;
			}

			if (!tk->IsSymbol('}')) {
				//REPORT_UNEXPECTED_TOKEN(PEParseDeclarationDeclExpected);
				//REPORT_UNEXPECTED(PEDeclSkipped);
				//OUTPUT_ERROR();
			}
			// Not a declaration...
			UngetToken();
			return false;
		}

		// Map property name to its ID and then parse the property
		StyleProperty propID = StyleFindProperty(propertyName.c_str());
		if (Style_UNKNOWN == propID) {
			return false;
		}

		//ParseProperty
		StyleValue* value = NULL;
		if (!ParseProperty(propID, value)) {
			// XXX Much better to put stuff in the value parsers instead...
			//REPORT_UNEXPECTED_P(PEValueParsingError, propertyName);
			//REPORT_UNEXPECTED(PEDeclDropped);
			//OUTPUT_ERROR();
			//mTempData.ClearProperty(propID);
			//mTempData.AssertInitialState();
			return false;
		}

		scoped_refptr<StyleValue> guard(value);

		// Look for a semicolon or close brace.
		if (!GetToken(true)) {
			// EOF is always ok
		}
		else if (token_.IsSymbol(';')) {
			// semicolon is always ok
		}
		else if (token_.IsSymbol('}')) {
			// brace is ok if checkForBraces, but don't eat it
			UngetToken();
			if (!checkForBraces) {
				return false;
			}
		}
		else {
			UngetToken();
			return false;
		}

		aList->Insert(new StyleDeclaration(propID, value));
		return true;
	}


	bool StyleParser::ParseProperty(StyleProperty aPropID, StyleValue*& v)
	{
		bool result;
		switch (aPropID) {
// 		case CSS_PROPERTY_PARSE_FUNCTION: {
// 				result = ParsePropertyByFunction(aPropID);
// 				break;
// 			}
		case Style_Width: {
				result = false;
				scoped_refptr<StyleValue> value(new StyleValue);
				if (ParseSingleValueProperty(aPropID, value.get())) {
					if (ExpectEndProperty()) {
						v = value.get();
						v->AddRef();
						result = true;
					}
					// XXX Report errors?
				}
				// XXX Report errors?
				break;
			}
// 			case CSS_PROPERTY_PARSE_VALUE_LIST: {
// 				result = ParseValueList(aPropID);
// 				break;
// 			}
		default: {
				result = false;
				//NS_ABORT_IF_FALSE(false,
				//	"Property's flags field in nsCSSPropList.h is missing "
				//	"one of the CSS_PROPERTY_PARSE_* constants");
				break;
			}
		}

		return result;
	}


	bool StyleParser::ParseSingleValueProperty(StyleProperty aPropID, StyleValue* aValue)
	{
#if 0
		if (aPropID == eCSSPropertyExtra_x_none_value) {
			return ParseVariant(aValue, VARIANT_NONE | VARIANT_INHERIT, nullptr);
		}

		if (aPropID == eCSSPropertyExtra_x_auto_value) {
			return ParseVariant(aValue, VARIANT_AUTO | VARIANT_INHERIT, nullptr);
		}

		if (aPropID < 0 || aPropID >= eCSSProperty_COUNT_no_shorthands) {
			NS_ABORT_IF_FALSE(false, "not a single value property");
			return false;
		}

		if (nsCSSProps::PropHasFlags(aPropID, CSS_PROPERTY_VALUE_PARSER_FUNCTION)) {
			switch (aPropID) {
			case eCSSProperty_font_family:
				return ParseFamily(aValue);
			case eCSSProperty_font_feature_settings:
				return ParseFontFeatureSettings(aValue);
			case eCSSProperty_font_weight:
				return ParseFontWeight(aValue);
			case eCSSProperty_marks:
				return ParseMarks(aValue);
			case eCSSProperty_text_decoration_line:
				return ParseTextDecorationLine(aValue);
			case eCSSProperty_text_overflow:
				return ParseTextOverflow(aValue);
			default:
				NS_ABORT_IF_FALSE(false, "should not reach here");
				return false;
			}
		}

		uint32 variant = nsCSSProps::ParserVariant(aPropID);
		if (variant == 0) {
			NS_ABORT_IF_FALSE(false, "not a single value property");
			return false;
		}

		// We only allow 'script-level' when unsafe rules are enabled, because
		// otherwise it could interfere with rulenode optimizations if used in
		// a non-MathML-enabled document.
		if (aPropID == eCSSProperty_script_level && !mUnsafeRulesEnabled)
			return false;

		const int32_t *kwtable = nsCSSProps::kKeywordTableTable[aPropID];
		switch (nsCSSProps::ValueRestrictions(aPropID)) {
		default:
			NS_ABORT_IF_FALSE(false, "should not be reached");
		case 0:
			return ParseVariant(aValue, variant, kwtable);
		case CSS_PROPERTY_VALUE_NONNEGATIVE:
			return ParseNonNegativeVariant(aValue, variant, kwtable);
		case CSS_PROPERTY_VALUE_AT_LEAST_ONE:
			return ParseOneOrLargerVariant(aValue, variant, kwtable);
		}
#endif
		return false;
	}


	void StyleParser::SkipRuleSet(bool aInsideBraces)
	{
		StyleToken* tk = &token_;
		for (;;) {
			if (!GetToken(true)) {
				//REPORT_UNEXPECTED_EOF(PESkipRSBraceEOF);
				break;
			}
			if (Token_Symbol == tk->mType) {
				char symbol = tk->mSymbol;
				if ('}' == symbol && aInsideBraces) {
					// leave block closer for higher-level grammar to consume
					UngetToken();
					break;
				}
				else if ('{' == symbol) {
					SkipUntil('}');
					break;
				}
				else if ('(' == symbol) {
					SkipUntil(')');
				}
				else if ('[' == symbol) {
					SkipUntil(']');
				}
			}
			else if (Token_Function == tk->mType ||
				Token_Bad_URL == tk->mType) {
				SkipUntil(')');
			}
		}
	}

	bool StyleParser::SkipUntil(char aStopSymbol)
	{
		StyleToken* tk = &token_;
		std::stack<char> cstack;
		cstack.push(aStopSymbol);
		for (;;) {
			if (!GetToken(true)) {
				return false;
			}
			if (Token_Symbol == tk->mType) {
				char symbol = tk->mSymbol;
				if (symbol == cstack.top()) {
					cstack.pop();
					if (cstack.empty()) {
						return true;
					}

					// Just handle out-of-memory by parsing incorrectly.  It's
					// highly unlikely we're dealing with a legitimate style sheet
					// anyway.
				}
				else if ('{' == symbol) {
					cstack.push('}');
				}
				else if ('[' == symbol) {
					cstack.push(']');
				}
				else if ('(' == symbol) {
					cstack.push(')');
				}
			}
			else if (Token_Function == tk->mType ||
				Token_Bad_URL == tk->mType) {
				cstack.push(')');
			}
		}
	}


	bool StyleParser::SkipDeclaration(bool aCheckForBraces)
	{
		StyleToken* tk = &token_;
		for (;;) {
			if (!GetToken(true)) {
				if (aCheckForBraces) {
					//REPORT_UNEXPECTED_EOF(PESkipDeclBraceEOF);
				}
				return false;
			}
			if (Token_Symbol == tk->mType) {
				char symbol = tk->mSymbol;
				if (';' == symbol) {
					break;
				}
				if (aCheckForBraces) {
					if ('}' == symbol) {
						UngetToken();
						break;
					}
				}
				if ('{' == symbol) {
					SkipUntil('}');
				}
				else if ('(' == symbol) {
					SkipUntil(')');
				}
				else if ('[' == symbol) {
					SkipUntil(']');
				}
			}
			else if (Token_Function == tk->mType ||
				Token_Bad_URL == tk->mType) {
				SkipUntil(')');
			}
		}
		return true;
	}


	bool StyleParser::ExpectSymbol(char aSymbol, bool aSkipWS)
	{
		if (!GetToken(aSkipWS)) {
			// CSS2.1 specifies that all "open constructs" are to be closed at
			// EOF.  It simplifies higher layers if we claim to have found an
			// ), ], }, or ; if we encounter EOF while looking for one of them.
			// Do still issue a diagnostic, to aid debugging.
			if (aSymbol == ')' || aSymbol == ']' ||
				aSymbol == '}' || aSymbol == ';') {
				//REPORT_UNEXPECTED_EOF_CHAR(aSymbol);
				return true;
			}
			else
				return false;
		}
		if (token_.IsSymbol(aSymbol)) {
			return true;
		}
		UngetToken();
		return false;
	}


	bool StyleParser::ExpectEndProperty()
	{
		if (CheckEndProperty())
			return true;

		// If we're here, we read something incorrect, so we should report it.
		//REPORT_UNEXPECTED_TOKEN(PEExpectEndValue);
		return false;
	}

	bool StyleParser::CheckEndProperty()
	{
		if (!GetToken(true)) {
			return true; // properties may end with eof
		}
		if ((Token_Symbol == token_.mType) &&
			((';' == token_.mSymbol) ||
			('!' == token_.mSymbol) ||
			('}' == token_.mSymbol) ||
			(')' == token_.mSymbol))) {
			// XXX need to verify that ! is only followed by "important [;|}]
			// XXX this requires a multi-token pushback buffer
			UngetToken();
			return true;
		}
		UngetToken();
		return false;
	}


}