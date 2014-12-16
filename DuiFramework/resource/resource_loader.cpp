#include "stdafx.h"
#include "resource_loader.h"
#include "ui_parser.h"

#include "framework/app.h"
#include "utils/utils.h"
#include "style/style_parser.h"


namespace ui
{
	ResourceLoader* ResourceLoader::Get()
	{
		return App::Get()->GetResourceLoader();
	}

	ResourceLoader::ResourceLoader()
	{
		base_path_ = dirname(App::Get()->GetAppPath());
	}

	ResourceLoader::~ResourceLoader()
	{

	}

	void ResourceLoader::Rebase(const std::wstring& path)
	{
		assert(isdir(path));
		base_path_ = path;
	}

	std::wstring ResourceLoader::ResolvePath(const std::wstring& relative) const
	{
		return pathcombine(base_path_, relative);
	}

	bool ResourceLoader::LoadFileToString(const std::wstring& relative, std::string* contnets)
	{
		return ReadFileToString(ResolvePath(relative), contnets);
	}

	bool ResourceLoader::LoadFileToString(const URL& url, std::string* contents)
	{
		if (!url.is_valid() || url.is_empty())
			return false;

		std::wstring real_path;
		if (url.SchemeIs("res")) {
			real_path = pathcombine(base_path_, MultiByteToWide(url.path().substr(1)));
		}
		else if (url.SchemeIs("file")) {
			real_path = MultiByteToWide(url.path());
		}
		else {
			return false;//其它不支持
		}

		if (!LoadFileToString(real_path, contents))
			return false;
		return true;
	}

	scoped_refptr<StyleSheet> ResourceLoader::GetStyleSheet(const ResourceId& id)
	{
		return GetStyleSheet(ResolveId(id));
	}

	scoped_refptr<StyleSheet> ResourceLoader::GetStyleSheet(const URL& url)
	{
		std::string contents;
		if (!LoadFileToString(url, &contents))
			return NULL;

		StyleParser parser;
		scoped_refptr<StyleSheet> sheet(new StyleSheet(url));
		parser.SetStyleSheet(sheet.get());
		if (!parser.ParseSheet(contents, 0))
			return NULL;
		return sheet;
	}

	ui::UIDocumentPtr ResourceLoader::GetUIDocument(const ResourceId& id)
	{
		return GetUIDocument(ResolveId(id));
	}

	ui::UIDocumentPtr ResourceLoader::GetUIDocument(const URL& url)
	{
		std::string contents;
		if (!LoadFileToString(url, &contents))
			return NULL;

		UIParser parser;
		UIDocumentPtr document(new UIDocument(url));
		parser.SetDomDocument(document);
		if (!parser.ParseXmlDocument(contents))
			return NULL;
		return document;
	}

	URL ResourceLoader::ResolveId(const ResourceId& id)
	{
		//TODO: 实现
		assert(0);
		return URL();
	}

}