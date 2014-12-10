#include "stdafx.h"
#include "style_parser.h"
#include "style_scanner.h"
#include "core/constants.h"
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

	static const int kBorderSidePropertySize = 2;

	static const StyleProperty kBorderLeftIDs[] = {
		Style_BorderLeftWidth,
		Style_BorderLeftColor,
	};

	static const StyleProperty kBorderTopIDs[] = {
		Style_BorderTopWidth,
		Style_BorderTopColor,
	};

	static const StyleProperty kBorderRightIDs[] = {
		Style_BorderRightWidth,
		Style_BorderRightColor,
	};

	static const StyleProperty kBorderBottomIDs[] = {
		Style_BorderBottomWidth,
		Style_BorderBottomColor,
	};

	static const StyleProperty kBorderWidthIDs[] = {
		Style_BorderLeftWidth,
		Style_BorderTopWidth,
		Style_BorderRightWidth,
		Style_BorderBottomWidth,
	};

	static const StyleProperty kBorderColorIDs[] = {
		Style_BorderLeftColor,
		Style_BorderTopColor,
		Style_BorderRightColor,
		Style_BorderBottomColor,
	};

	static const StyleProperty kBorderRadiusIDs[] = {
		Style_BorderLeftTopRadius,
		Style_BorderRightTopRadius,
		Style_BorderRightBottomRadius,
		Style_BorderLeftBottomRadius,
	};

	static const StyleProperty kMarginIDs[] = {
		Style_MarginLeft,
		Style_MarginTop,
		Style_MarginRight,
		Style_MarginBottom,
	};

	StyleParser::StyleParser()
	{
		scanner_ = NULL;
		have_bush_back_ = false;
	}


	void StyleParser::SetStyleSheet(StyleSheet* sheet)
	{
		sheet_.reset(sheet);
	}


	bool StyleParser::ParseSheet(const std::string& buffer, uint32 aLineNumber)
	{
		StyleScanner scanner(buffer, 0);
		InitScanner(scanner);

		StyleToken* tk = &token_;
		for (;;) {
			// Get next non-whitespace token
			if (!GetToken(true)) {
				//OUTPUT_ERROR();
				break;
			}
			if (Token_HTMLComment == tk->mType) {
				continue; // legal here only
			}
			if (Token_AtKeyword == tk->mType) {
				//ParseAtRule(AppendRuleToSheet, this, false);
				continue;
			}
			UngetToken();
			if (ParseRuleSet(AppendRuleToSheet, this)) {
				//mSection = eCSSSection_General;
			}
		}
		ReleaseScanner();

		return true;
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
		if (!ParseSelector(selector)) {
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
					if (!ParseSelector(newList)) {
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


	bool StyleParser::ParseSelector(StyleSelector*& aList)
	{
		char combinator = 0;
		scoped_ptr<StyleSelector> list;

		for (;;) {
			StyleSelectorNode* selectorNode = NULL;
			if (!ParseSelectorNode(selectorNode, combinator)) {
				return false;
			}

			if (!list.get()) {
				list.reset(new StyleSelector);
			}
			list->AddChildSelector(selectorNode);

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
	bool StyleParser::ParseSelectorNode(StyleSelectorNode*& aList, char aPrevCombinator)
	{
		aList = NULL;

		if (!GetToken(true)) {
			//REPORT_UNEXPECTED_EOF(PESelectorEOF);
			return false;
		}
		
		StyleSelectorNode* selector = new StyleSelectorNode;
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
				success = ParseIDSelectorNode(selector);
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
				success = ParseClassSelectorNode(selector);
			}
			else if (token_.IsSymbol(':')) {    // :pseudo
				success = ParsePseudoSelectorNode(selector);
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
		if (sheet_.get())
			sheet_->AppendStyleRule(rule);
	}

	bool StyleParser::ParseIDSelectorNode(StyleSelectorNode* s)
	{
		s->SetId(token_.mIdent);
		return true;
	}

	bool StyleParser::ParseClassSelectorNode(StyleSelectorNode* s)
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

	bool StyleParser::ParsePseudoSelectorNode(StyleSelectorNode* s)
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
				bool changed = false;
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

		declaration_list_.Clear();
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
		if (!ParseProperty(propID)) {
			// XXX Much better to put stuff in the value parsers instead...
			//REPORT_UNEXPECTED_P(PEValueParsingError, propertyName);
			//REPORT_UNEXPECTED(PEDeclDropped);
			//OUTPUT_ERROR();
			//mTempData.ClearProperty(propID);
			//mTempData.AssertInitialState();
			declaration_list_.Remove(propID);
			return false;
		}

		//scoped_refptr<StyleValue> guard(value);

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

		declaration_list_.TransferTo(aList);
		return true;
	}


	bool StyleParser::ParseProperty(StyleProperty aPropID)
	{
		bool result = false;
		switch (StyleFindParseType(aPropID)) {
		case PROPERTY_PARSE_FUNCTION:
			result = ParsePropertyByFunction(aPropID);
			break;
		
		case PROPERTY_PARSE_VALUE_LIST: 
			result = ParseValueList(aPropID);
			//scoped_refptr<StyleValue> value(new StyleValue);
 			break;
 			
		case PROPERTY_PARSE_VALUE: {
			scoped_refptr<StyleValue> value(new StyleValue);
			if (ParseSingleValueProperty(aPropID, value.get())) {
				if (ExpectEndProperty()) {
					AppendValue(aPropID, value.get());
					result = true;
				}
				// XXX Report errors?
			}
			// XXX Report errors?
			break;
		}
		default: 
			//NS_ABORT_IF_FALSE(false,
			//	"Property's flags field in nsCSSPropList.h is missing "
			//	"one of the CSS_PROPERTY_PARSE_* constants");
			break;
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
#endif
		uint32 variant = StyleFindVariant(aPropID);
		if (variant == 0) {
			//NS_ABORT_IF_FALSE(false, "not a single value property");
			return false;
		}

#if 0
		// We only allow 'script-level' when unsafe rules are enabled, because
		// otherwise it could interfere with rulenode optimizations if used in
		// a non-MathML-enabled document.
		if (aPropID == eCSSProperty_script_level && !mUnsafeRulesEnabled)
			return false;

		const int32_t *kwtable = nsCSSProps::kKeywordTableTable[aPropID];
#endif
		switch (StyleFindValueRestrictions(aPropID)) {
		case 0:
			return ParseVariant(aValue, variant);
		case PROPERTY_VALUE_NONNEGATIVE:
			return ParseNonNegativeVariant(aValue, variant);
		case PROPERTY_VALUE_AT_LEAST_ONE:
			//return ParseOneOrLargerVariant(aValue, variant, kwtable);
		default:
			break;
		}

		return false;
	}


	bool StyleParser::ParseValueList(StyleProperty p)
	{
		scoped_ptr<StyleValueArray> arrayValue;
		for (;;) {
			scoped_refptr<StyleValue> v(new StyleValue);
			if (!ParseSingleValueProperty(p, v.get())) {
				return false;
			}
			if (CheckEndProperty()) {
				break;
			}
			if (!ExpectSymbol(',', true)) {
				return false;
			}
			arrayValue->Add(v.get());
		}
		scoped_refptr<StyleValue> value(new StyleValue);
		value->SetArrayValue(arrayValue.release());
		AppendValue(p, value.get());
		return true;
	}


	bool StyleParser::ParseVariant(StyleValue* aValue, int32 aVariantMask)
	{
		if (!GetToken(true)) {
			return false;
		}
		StyleToken* tk = &token_;

		if (((aVariantMask & (VARIANT_AUTO | VARIANT_NONE)) != 0) &&
			(Token_Ident == tk->mType)) {
			if ((aVariantMask & VARIANT_AUTO) != 0) {
				if ("auto" == tk->mIdent) {
					aValue->SetAutoValue();
					return true;
				}
			}
			if ((aVariantMask & VARIANT_NONE) != 0) {
				if ("none" == tk->mIdent) {
					//aValue->SetNoneValue();
					return true;
				}
			}
		}

		if (((aVariantMask & VARIANT_NUMBER) != 0) &&
			(Token_Number == tk->mType)) {
			aValue->SetNumberValue(tk->mNumber);
			return true;
		}
		if (((aVariantMask & VARIANT_INTEGER) != 0) &&
			(Token_Number == tk->mType) && tk->mIntegerValid) {
			aValue->SetIntValue(tk->mInteger);
			return true;
		}
		if (((aVariantMask & (VARIANT_LENGTH | VARIANT_ANGLE)) != 0 &&
			Token_Dimension == tk->mType) ||
			((aVariantMask & (VARIANT_LENGTH | VARIANT_ZERO_ANGLE)) != 0 &&
			Token_Number == tk->mType &&
			tk->mNumber == 0.0f)) {
			if (((aVariantMask & VARIANT_POSITIVE_DIMENSION) != 0 &&
				tk->mNumber <= 0.0) ||
				((aVariantMask & VARIANT_NONNEGATIVE_DIMENSION) != 0 &&
				tk->mNumber < 0.0)) {
				UngetToken();
				return false;
			}
			if (TranslateDimension(aValue, aVariantMask, tk->mNumber, tk->mIdent)) {
				return true;
			}
			// Put the token back; we didn't parse it, so we shouldn't consume it
			UngetToken();
			return false;
		}
		if (((aVariantMask & VARIANT_PERCENT) != 0) &&
			(Token_Percentage == tk->mType)) {
			aValue->SetPercentValue(tk->mNumber);
			return true;
		}

		if (((aVariantMask & VARIANT_URL) != 0) &&
			Token_URL == tk->mType) {
			SetValueToURL(aValue, tk->mIdent);
			return true;
		}

		if ((aVariantMask & VARIANT_GRADIENT) != 0 &&
			Token_Function == tk->mType) {
			ParseLinearGradient(aValue);
			return false;
		}

		if ((aVariantMask & VARIANT_COLOR) != 0) {
			if ((Token_ID == tk->mType) ||
				(Token_Hash == tk->mType) ||
				(Token_Ident == tk->mType) ||
				((Token_Function == tk->mType) &&
				(tk->mIdent == "rgb" ||
				tk->mIdent == "hsl" ||
				tk->mIdent == "rgba" ||
				tk->mIdent == "hsla")))
			{
				// Put token back so that parse color can get it
				UngetToken();
				if (ParseColor(aValue)) {
					return true;
				}
				return false;
			}
		}

		if (((aVariantMask & VARIANT_STRING) != 0) &&
			(Token_String == tk->mType)) {
			aValue->SetStringValue(tk->mIdent);
			return true;
		}

		UngetToken();
		return false;
	}

#define VARIANT_ALL_NONNUMERIC \
  VARIANT_COLOR | \
  VARIANT_URL | \
  VARIANT_STRING | \
  VARIANT_AUTO | \
  VARIANT_NONE | \
  VARIANT_GRADIENT

	bool StyleParser::ParseNonNegativeVariant(StyleValue* aValue, int32 aVariantMask)
	{
		if ((aVariantMask & ~(VARIANT_ALL_NONNUMERIC | VARIANT_NUMBER | VARIANT_LENGTH | VARIANT_PERCENT |
			VARIANT_INTEGER))) {
			return false;
		}

		if (ParseVariant(aValue, aVariantMask)) {
			if (StyleValue_Number == aValue->GetType()/* ||
				aValue.IsLengthUnit()*/){
				if (aValue->GetFloatValue() < 0) {
					UngetToken();
					return false;
				}
			}
			else if (aValue->GetType() == StyleValue_Percent) {
				if (aValue->GetPercentValue() < 0) {
					UngetToken();
					return false;
				}
			}
			else if (aValue->GetType() == StyleValue_Integer) {
				if (aValue->GetIntValue() < 0) {
					UngetToken();
					return false;
				}
			}
			return true;
		}
		return false;
	}

#define COLOR_TYPE_UNKNOWN 0
#define COLOR_TYPE_INTEGERS 1
#define COLOR_TYPE_PERCENTAGES 2

	bool StyleParser::ParseColor(StyleValue* aValue)
	{
		if (!GetToken(true)) {
			//REPORT_UNEXPECTED_EOF(PEColorEOF);
			return false;
		}

		StyleToken* tk = &token_;
		Color rgba;
		switch (tk->mType) {
		case Token_ID:
		case Token_Hash:
			// #xxyyzz
			if (HexToRGB(tk->mIdent, rgba)) {
				aValue->SetColorValue(rgba);
				return true;
			}
			break;

		case Token_Ident:
#if 0
			if (ColorNameToRGB(tk->mIdent, &rgba)) {
				aValue->SetColorValue(rgba);
				return true;
			}
			else {
				nsCSSKeyword keyword = nsCSSKeywords::LookupKeyword(tk->mIdent);
				if (eCSSKeyword_UNKNOWN < keyword) { // known keyword
					int32_t value;
					if (nsCSSProps::FindKeyword(keyword, nsCSSProps::kColorKTable, value)) {
						aValue.SetIntValue(value, eCSSUnit_EnumColor);
						return true;
					}
				}
			}
#endif
			break;
		case Token_Function:
			if (token_.mIdent == "rgb") {
				// rgb ( component , component , component )
				uint8 r, g, b;
				int32 type = COLOR_TYPE_UNKNOWN;
				if (ParseColorComponent(r, type, ',') &&
					ParseColorComponent(g, type, ',') &&
					ParseColorComponent(b, type, ')')) {
					aValue->SetColorValue(ColorSetRGB(r, g, b));
					return true;
				}
				SkipUntil(')');
				return false;
			}
			else if (token_.mIdent == "rgba") {
				// rgba ( component , component , component , opacity )
				uint8 r, g, b, a;
				int32 type = COLOR_TYPE_UNKNOWN;
				if (ParseColorComponent(r, type, ',') &&
					ParseColorComponent(g, type, ',') &&
					ParseColorComponent(b, type, ',') &&
					ParseColorOpacity(a)) {
					aValue->SetColorValue(ColorSetARGB(a, r, g, b));
					return true;
				}
				SkipUntil(')');
				return false;
			}
			else if (token_.mIdent == "hsl") {
				// hsl ( hue , saturation , lightness )
				// "hue" is a number, "saturation" and "lightness" are percentages.
				if (ParseHSLColor(rgba, ')')) {
					aValue->SetColorValue(rgba);
					return true;
				}
				SkipUntil(')');
				return false;
			}
			else if (token_.mIdent == "hsla") {
				// hsla ( hue , saturation , lightness , opacity )
				// "hue" is a number, "saturation" and "lightness" are percentages,
				// "opacity" is a number.
				uint8 a;
				if (ParseHSLColor(rgba, ',') &&
					ParseColorOpacity(a)) {
					aValue->SetColorValue(ColorSetARGB(a, ColorGetR(rgba), ColorGetG(rgba),
						ColorGetB(rgba)));
					return true;
				}
				SkipUntil(')');
				return false;
			}
			break;
		default:
			break;
		}

		UngetToken();
		return false;
	}


	bool StyleParser::ParseColorComponent(uint8& aComponent, int32& aType, char aStop)
	{
		if (!GetToken(true)) {
			//REPORT_UNEXPECTED_EOF(PEColorComponentEOF);
			return false;
		}
		float value;
		StyleToken* tk = &token_;
		switch (tk->mType) {
		case Token_Number:
			switch (aType) {
			case COLOR_TYPE_UNKNOWN:
				aType = COLOR_TYPE_INTEGERS;
				break;
			case COLOR_TYPE_INTEGERS:
				break;
			case COLOR_TYPE_PERCENTAGES:
				//REPORT_UNEXPECTED_TOKEN(PEExpectedPercent);
				UngetToken();
				return false;
			default:
				//NS_NOTREACHED("Someone forgot to add the new color component type in here");
				break;
			}

			if (!token_.mIntegerValid) {
				//REPORT_UNEXPECTED_TOKEN(PEExpectedInt);
				UngetToken();
				return false;
			}
			value = tk->mNumber;
			break;
		case Token_Percentage:
			switch (aType) {
			case COLOR_TYPE_UNKNOWN:
				aType = COLOR_TYPE_PERCENTAGES;
				break;
			case COLOR_TYPE_INTEGERS:
				//REPORT_UNEXPECTED_TOKEN(PEExpectedInt);
				UngetToken();
				return false;
			case COLOR_TYPE_PERCENTAGES:
				break;
			default:
				//NS_NOTREACHED("Someone forgot to add the new color component type in here");
				break;
			}
			value = tk->mNumber * 255.0f;
			break;
		default:
			//REPORT_UNEXPECTED_TOKEN(PEColorBadRGBContents);
			UngetToken();
			return false;
		}
		if (ExpectSymbol(aStop, true)) {
			if (value < 0.0f) value = 0.0f;
			if (value > 255.0f) value = 255.0f;
			aComponent = std::round(value);
			return true;
		}
		//REPORT_UNEXPECTED_TOKEN_CHAR(PEColorComponentBadTerm, aStop);
		return false;
	}


	bool StyleParser::ParseColorOpacity(uint8& aOpacity)
	{
		if (!GetToken(true)) {
			//REPORT_UNEXPECTED_EOF(PEColorOpacityEOF);
			return false;
		}

		if (token_.mType != Token_Number) {
			//REPORT_UNEXPECTED_TOKEN(PEExpectedNumber);
			UngetToken();
			return false;
		}

		if (token_.mNumber < 0.0f) {
			token_.mNumber = 0.0f;
		}
		else if (token_.mNumber > 1.0f) {
			token_.mNumber = 1.0f;
		}

		uint8 value = std::round(token_.mNumber*255);
		// Need to compare to something slightly larger
		// than 0.5 due to floating point inaccuracies.
		//NS_ASSERTION(fabs(255.0f*mToken.mNumber - value) <= 0.51f,
		//	"FloatToColorComponent did something weird");

		if (!ExpectSymbol(')', true)) {
			//REPORT_UNEXPECTED_TOKEN(PEExpectedCloseParen);
			return false;
		}

		aOpacity = value;

		return true;
	}


	bool StyleParser::ParseHSLColor(Color& aColor, char aStop)
	{
		float h, s, l;

		// Get the hue
		if (!GetToken(true)) {
			//REPORT_UNEXPECTED_EOF(PEColorHueEOF);
			return false;
		}
		if (token_.mType != Token_Number) {
			//REPORT_UNEXPECTED_TOKEN(PEExpectedNumber);
			UngetToken();
			return false;
		}
		h = token_.mNumber;
		h /= 360.0f;
		// hue values are wraparound
		h = h - floor(h);

		if (!ExpectSymbol(',', true)) {
			//REPORT_UNEXPECTED_TOKEN(PEExpectedComma);
			return false;
		}

		// Get the saturation
		if (!GetToken(true)) {
			//REPORT_UNEXPECTED_EOF(PEColorSaturationEOF);
			return false;
		}
		if (token_.mType != Token_Percentage) {
			//REPORT_UNEXPECTED_TOKEN(PEExpectedPercent);
			UngetToken();
			return false;
		}
		s = token_.mNumber;
		if (s < 0.0f) s = 0.0f;
		if (s > 1.0f) s = 1.0f;

		if (!ExpectSymbol(',', true)) {
			//REPORT_UNEXPECTED_TOKEN(PEExpectedComma);
			return false;
		}

		// Get the lightness
		if (!GetToken(true)) {
			//REPORT_UNEXPECTED_EOF(PEColorLightnessEOF);
			return false;
		}
		if (token_.mType != Token_Percentage) {
			//REPORT_UNEXPECTED_TOKEN(PEExpectedPercent);
			UngetToken();
			return false;
		}
		l = token_.mNumber;
		if (l < 0.0f) l = 0.0f;
		if (l > 1.0f) l = 1.0f;

		if (ExpectSymbol(aStop, true)) {
			aColor = HSL2RGB(h, s, l);
			return true;
		}

		//REPORT_UNEXPECTED_TOKEN_CHAR(PEColorComponentBadTerm, aStop);
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

	bool StyleParser::TranslateDimension(StyleValue* aValue, int32 aVariantMask, float aNumber, const std::string& aUnit)
	{
		StyleValueType units;
		int32 type = 0;
		if (!aUnit.empty()) {
			if (aUnit == "px") {
				type = VARIANT_LENGTH;
				units = StyleValue_Pixel;
			}
			else if (aUnit == "deg") {
				type = VARIANT_ANGLE;
				units = StyleValue_Degree;
			}
			else {
				// Unknown unit
				return false;
			}
		}
		else {
			// Must be a zero number...
			assert(0 == aNumber);
			if ((VARIANT_LENGTH & aVariantMask) != 0) {
				units = StyleValue_Pixel;
				type = VARIANT_LENGTH;
			}
			else if ((VARIANT_ANGLE & aVariantMask) != 0) {
				assert(aVariantMask & VARIANT_ZERO_ANGLE);
				units = StyleValue_Degree;
				type = VARIANT_ANGLE;
			}
			else {
				//NS_ERROR("Variant mask does not include dimension; why were we called?");
				return false;
			}
		}
		if ((type & aVariantMask) != 0) {
			aValue->SetFloatValue(aNumber, units);
			return true;
		}
		return false;
	}

	void StyleParser::SetValueToURL(StyleValue* v, const std::string& str)
	{
		if (sheet_) {
			v->SetUrlValue(sheet_->url().Resolve(str));
		}
		else {
			v->SetUrlValue(URL(str));
		}
	}

	bool StyleParser::ParseLinearGradient(StyleValue* v)
	{
		return false;
	}

	bool StyleParser::ParsePropertyByFunction(StyleProperty p)
	{
		switch (p) {
		case Style_Cursor:
			return ParseCursor();
		case Style_Border:
			return ParseBorder();
		case Style_BorderWidth:
			return ParseBorderWidth();
		case Style_BorderColor:
			return ParseBorderColor();
		case Style_BorderRadius:
			return ParseBorderRadius();
		case Style_BorderLeft:
			return ParseBorderSide(kBorderLeftIDs);
		case Style_BorderTop:
			return ParseBorderSide(kBorderTopIDs);
		case Style_BorderRight:
			return ParseBorderSide(kBorderRightIDs);
		case Style_BorderBottom:
			return ParseBorderSide(kBorderBottomIDs);
		case Style_Margin:
			return ParseMargin();
		default:
			return false;
		}
	}

	bool StyleParser::ParseCursor()
	{
		if (!GetToken(true)) {
			//ÂÔÈ¥:
			return false;
		}

		scoped_refptr<StyleValue> v(new StyleValue);
		StyleToken* tk = &token_;
		if (tk->mIdent == "hand") {
			v->SetCursorValue(Cursor_Hand);
		}
		else if (tk->mIdent == "arrow") {
			v->SetCursorValue(Cursor_Arrow);
		}
		else {
			v->SetAutoValue();
		}

		AppendValue(Style_Cursor, v.get());
		return true;
	}

	

	bool StyleParser::ParseBorder()
	{
		scoped_refptr<StyleValue> values[kBorderSidePropertySize];
		for (int i = 0; i < kBorderSidePropertySize; i++) {
			values[i].reset(new StyleValue);
		}
		int found = ParseChoice(kBorderLeftIDs, values, kBorderSidePropertySize);
		if ((found < 1) || (false == ExpectEndProperty())) {
			return false;
		}

		for (int index = 0; index < 4; index++) {
			AppendValue(kBorderWidthIDs[index], values[0].get());
			AppendValue(kBorderColorIDs[index], values[1].get());
		}
		return true;
	}


	bool StyleParser::ParseBorderSide(const StyleProperty propertys[])
	{
		scoped_refptr<StyleValue> values[kBorderSidePropertySize];
		for (int i = 0; i < kBorderSidePropertySize; i++) {
			values[i].reset(new StyleValue);
		}
		int found = ParseChoice(propertys, values, kBorderSidePropertySize);
		if ((found < 1) || (false == ExpectEndProperty())) {
			return false;
		}

		for (int index = 0; index < kBorderSidePropertySize; index++) {
			AppendValue(propertys[index], values[index].get());
		}

		return true;
	}


	int StyleParser::ParseChoice(const StyleProperty aPropIDs[], scoped_refptr<StyleValue> aValues[], int aNumIDs)
	{
		int32 found = 0;
		int32 loop;
		for (loop = 0; loop < aNumIDs; loop++) {
			// Try each property parser in order
			int32 hadFound = found;
			int32 index;
			for (index = 0; index < aNumIDs; index++) {
				int32 bit = 1 << index;
				if ((found & bit) == 0) {
					if (ParseSingleValueProperty(aPropIDs[index], aValues[index].get())) {
						found |= bit;
						// It's more efficient to break since it will reset |hadFound|
						// to |found|.  Furthermore, ParseListStyle depends on our going
						// through the properties in order for each value..
						break;
					}
				}
			}
			if (found == hadFound) {  // found nothing new
				break;
			}
		}
		if (0 < found) {
			if (1 == found) { // only first property
				if (aValues[0]->IsAutoValue()) {// one auto, all auto
					for (loop = 1; loop < aNumIDs; loop++) {
						aValues[loop]->SetAutoValue();
					}
					found = ((1 << aNumIDs) - 1);
				}
			}
			else {  // more than one value, verify no auto
				for (loop = 0; loop < aNumIDs; loop++) {
					if (aValues[loop]->IsAutoValue()) {
						found = -1;
						break;
					}
				}
			}
		}
		return found;
	}

	void StyleParser::AppendValue(StyleProperty p, StyleValue* v)
	{
		declaration_list_.Insert(new StyleDeclaration(p, v));
	}

	bool StyleParser::ParseBorderWidth()
	{
		return ParseBoxProperties(kBorderWidthIDs);
	}

	bool StyleParser::ParseBorderColor()
	{
		return ParseBoxProperties(kBorderColorIDs);
	}

	//left, top, right, bottom 
	bool StyleParser::ParseBoxProperties(const StyleProperty propertys[])
	{
		int count = 0;
		scoped_refptr<StyleValue> values[4];
		for (int i = 0; i < 4; i++) {
			values[i].reset(new StyleValue);
			if (!ParseSingleValueProperty(propertys[i], values[i].get())) {
				break;
			}
			count++;
		}

		if (count == 0)
			return false;

		switch (count)
		{
		case 1: // Make top == left
			values[1] = values[0];
		case 2: // Make right == left
			values[2] = values[0];
		case 3: // Make bottom == top
			values[3] = values[1];
		default:
			break;
		}

		for (int i = 0; i < 4; i++) {
			AppendValue(propertys[i], values[i].get());
		}
		return true;
	}

	bool StyleParser::ParseBorderRadius()
	{
		return ParseBoxProperties(kBorderRadiusIDs);
	}

	bool StyleParser::ParseMargin()
	{
		return ParseBoxProperties(kMarginIDs);
	}

}


