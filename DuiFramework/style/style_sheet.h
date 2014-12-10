#pragma once
#include "style/style_rule.h"
#include "style/style_result.h"

namespace ui
{
	class StyleSheet : public RefCounted<StyleSheet>
	{
	public:
		StyleSheet(const URL& url);
		~StyleSheet();

		//static scoped_refptr<StyleSheet> LoadFromFile(const std::wstring& path);

		void AppendStyleRule(StyleRule* rule);

		void SelectStyles(UIElement* e, StyleResultList* s);

		int32 StyleRuleCount() const;

		const URL& url() const { return url_; }
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
		void SelectStyles(UIElement* e, StyleResultList* s);
	private:
		std::vector<StyleSheet*> sheets_;
		DISALLOW_COPY_AND_ASSIGN(StyleSheetList);
	};
}