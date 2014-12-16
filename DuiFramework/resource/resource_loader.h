#pragma once
#include "base/basictypes.h"
#include "style/style_sheet.h"
#include "ui/ui_document.h"

namespace ui
{
	typedef std::string ResourceId;
	//资源类型
	// layout xml
	// resource define xml
	// style sheet
	class ResourceLoader
	{
	public:
		static ResourceLoader* Get();

		//for debug
		void Rebase(const std::wstring& path);

		URL ResolveId(const ResourceId& id);
		std::wstring ResolvePath(const std::wstring& relative) const;

		scoped_refptr<StyleSheet> GetStyleSheet(const ResourceId& id);
		scoped_refptr<StyleSheet> GetStyleSheet(const URL& url);
		
		UIDocumentPtr GetUIDocument(const ResourceId& id);
		UIDocumentPtr GetUIDocument(const URL& url);

		bool LoadFileToString(const URL& url, std::string* contnets);
		bool LoadFileToString(const std::wstring& relative, std::string* contnets);

	private:
		friend class App;
		friend class scoped_ptr<ResourceLoader>;
		ResourceLoader();
		~ResourceLoader();

		std::wstring base_path_;
	};
}