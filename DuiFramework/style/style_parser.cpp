#include "stdafx.h"
#include "style_parser.h"
#include "style_scanner.h"

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
			//SkipRuleSet(aInsideBraces);
			return false;
		}
		//NS_ASSERTION(nullptr != slist, "null selector list");
		//CLEAR_ERROR();
#if 0
		// Next parse the declaration block
		uint32_t parseFlags = eParseDeclaration_InBraces |
			eParseDeclaration_AllowImportant;
		css::Declaration* declaration = ParseDeclarationBlock(parseFlags);
		if (nullptr == declaration) {
			delete slist;
			return false;
		}

		// Translate the selector list and declaration block into style data

		nsRefPtr<css::StyleRule> rule = new css::StyleRule(slist, declaration);
		rule->SetLineNumber(linenum);
		(*aAppendFunc)(rule, aData);
#endif
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
		scoped_ptr<StyleSelector> list(new StyleSelector());

		for (;;) {
			if (!ParseSelector(list.get(), combinator)) {
				return false;
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
	bool StyleParser::ParseSelector(StyleSelector* aList, char aPrevCombinator)
	{
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
			aList->AddChildSelector(selector);
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

}