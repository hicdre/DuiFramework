#pragma once
#include "style/style_sheet.h"
#include "style/style_scanner.h"


namespace ui
{
	typedef void(*RuleAppendFunc) (StyleRule* aRule, void* aData);

	class StyleParser
	{
	public:
		StyleParser();

		void SetStyleSheet(StyleSheet* sheet);

		bool ParseSheet(const std::string& buffer, uint32 aLineNumber);
		bool ParseRule(const std::string& buffer, StyleRule** rule);

	public:
		bool ParseRuleSet(RuleAppendFunc aAppendFunc, void* aData,
			bool aInsideBraces = false);

		bool ParseSelectorList(StyleSelectorList*& aListHead,
			char aStopChar);
		bool ParseSelector(StyleSelector*& aList);
		bool ParseSelectorNode(StyleSelectorNode*& aList,
			char aPrevCombinator);
		bool ParseIDSelectorNode(StyleSelectorNode* s);
		bool ParseClassSelectorNode(StyleSelectorNode* s);
		bool ParsePseudoSelectorNode(StyleSelectorNode* s);

		bool ParseDeclarationBlock(StyleDeclarationList*& aList, bool inbraces);
		bool ParseDeclaration(StyleDeclarationList* aList, bool inbraces,
			bool aMustCallValueAppended,
			bool* aChanged);
		bool ParseProperty(StyleProperty p, StyleValue*& v);
		bool ParsePropertyByFunction(StyleProperty p, StyleValue* v);
		bool ParseSingleValueProperty(StyleProperty p, StyleValue* v);
		bool ParseValueList(StyleProperty p, StyleValueArray* a);
		bool ParseVariant(StyleValue* v, int32 aVariantMask);
		bool ParseNonNegativeVariant(StyleValue* v, int32 aVariantMask);

		bool ParseColor(StyleValue* v);
		bool ParseColorComponent(uint8& aComponent, int32& aType, char aStop);
		bool ParseColorOpacity(uint8& aOpacity);
		bool ParseHSLColor(Color& aColor, char aStop);

		bool ParseLinearGradient(StyleValue* v);
		bool ParseCursor(StyleValue* v);

		void SetValueToURL(StyleValue* v, const std::string& str);

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

		bool TranslateDimension(StyleValue* aValue, int32 aVariantMask, float aNumber,
			const std::string& aUnit);
	private:
		StyleToken token_;
		StyleScanner* scanner_;
		scoped_refptr<StyleSheet> sheet_;
		bool have_bush_back_;
		DISALLOW_COPY_AND_ASSIGN(StyleParser);
	};
}