#include "stdafx.h"
#include "style_sheet.h"
#include "utils/utils.h"
#include "utils/resource_loader.h"
#include "style_parser.h"

namespace ui
{

	StyleSheet::StyleSheet(const URL& url)
		: url_(url)
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

	void StyleSheet::SelectStyles(UIElement* e, StyleResultList* s)
	{
		for (StyleRule* rule : rules_)
		{
			rule->SelectStyles(e, s);
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

	void StyleSheetList::SelectStyles(UIElement* e, StyleResultList* s)
	{
		for (StyleSheet* sheet : sheets_)
		{
			sheet->SelectStyles(e, s);
		}
	}

}