#include "stdafx.h"
#include "style_sheet.h"
#include "utils/utils.h"
#include "style_parser.h"

namespace ui
{


	StyleSheet::StyleSheet()
	{

	}

	StyleSheet::~StyleSheet()
	{
		for (StyleRule* r : rules_)
		{
			r->SetStyleSheet(NULL);
			r->Release();
		}
		rules_.clear();
	}

	void StyleSheet::AppendStyleRule(StyleRule* rule)
	{
		rules_.push_back(rule);
		rule->AddRef();
		rule->SetStyleSheet(this);
	}

	int32 StyleSheet::StyleRuleCount() const
	{
		return rules_.size();
	}

	scoped_refptr<StyleSheet> StyleSheet::LoadFromFile(const std::wstring& path)
	{
		std::string contents;
		if (!ReadFileToString(path, &contents))
			return NULL;

		StyleParser parser;
		scoped_refptr<StyleSheet> sheet(new StyleSheet);
		parser.SetStyleSheet(sheet.get());
		if (!parser.ParseSheet(contents, 0))
			return NULL;
		return sheet;
	}

	void StyleSheet::MatchRules(View* v, StyleDeclarationList& r)
	{
		for (StyleRule* rule : rules_)
		{
			rule->MatchRules(v, r);
		}
	}



	StyleSheetList::StyleSheetList()
	{

	}

	StyleSheetList::~StyleSheetList()
	{
		for (StyleSheet* sheet : sheets_)
		{
			sheet->Release();
		}
	}

	void StyleSheetList::AddStyleSheet(StyleSheet* sheet)
	{
		sheets_.push_back(sheet);
		sheet->AddRef();
	}

	void StyleSheetList::MatchRules(View* v, StyleDeclarationList& r)
	{
		for (StyleSheet* sheet : sheets_)
		{
			sheet->MatchRules(v, r);
		}
	}

}