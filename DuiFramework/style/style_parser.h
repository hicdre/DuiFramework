#pragma once
#include "style/style_rule.h"
#include "style/style_scanner.h"

namespace ui
{
	typedef void(*RuleAppendFunc) (StyleRule* aRule, void* aData);

	class StyleParser
	{
	public:
		StyleParser();

		bool ParseRule(const std::string& buffer, StyleRule** rule);

		bool ParseRuleSet(RuleAppendFunc aAppendFunc, void* aData,
			bool aInsideBraces = false);

		bool ParseSelectorList(StyleSelectorList*& aListHead,
			char aStopChar);
		bool ParseSelectorGroup(StyleSelector*& aList);
		bool ParseSelector(StyleSelector*& aList,
			char aPrevCombinator);
		bool ParseIDSelector(StyleSelector* s);
		bool ParseClassSelector(StyleSelector* s);
		bool ParsePseudoSelector(StyleSelector* s);

		bool ParseDeclarationBlock(StyleDeclarationList*& aList, bool inbraces);
		bool ParseDeclaration(StyleDeclarationList* aList, bool inbraces,
			bool aMustCallValueAppended,
			bool* aChanged);
		bool ParseProperty(StyleProperty p, StyleValue*& v);
		bool ParseSingleValueProperty(StyleProperty p, StyleValue* v);

		void AppendRule(StyleRule* rule);

		void InitScanner(StyleScanner& scanner);
		void ReleaseScanner();

		bool GetToken(bool aSkipWS);
		void UngetToken();

		void SkipRuleSet(bool aInsideBraces);
		bool SkipUntil(char aStopSymbol);
		bool SkipDeclaration(bool aCheckForBraces);

		bool ExpectSymbol(char aSymbol, bool aSkipWS);
		bool ExpectEndProperty();
		bool CheckEndProperty();
	private:
		StyleToken token_;
		StyleScanner* scanner_;
		bool have_bush_back_;
		DISALLOW_COPY_AND_ASSIGN(StyleParser);
	};
}