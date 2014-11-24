#pragma once
#include "style/style_rule.h"

namespace ui
{
	class StyleSheet : public RefCounted<StyleSheet>
	{
	public:
		StyleSheet(const URL& url);
		~StyleSheet();

		//static scoped_refptr<StyleSheet> LoadFromFile(const std::wstring& path);

		void AppendStyleRule(StyleRule* rule);

		void MatchRules(View* v, StyleDeclarationList& r);

		int32 StyleRuleCount() const;
	private:
		std::vector<StyleRule*> rules_;
		URL url_;
		DISALLOW_COPY_AND_ASSIGN(StyleSheet);
	};

	class StyleSheetList
	{
	public:
		StyleSheetList();
		~StyleSheetList();

		void AddStyleSheet(StyleSheet* sheet);
		void MatchRules(View* v, StyleDeclarationList& r);
	private:
		std::vector<StyleSheet*> sheets_;
		DISALLOW_COPY_AND_ASSIGN(StyleSheetList);
	};
}