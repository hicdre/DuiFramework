#pragma once

#include "base/basictypes.h"
#include <string>
#include <functional>


namespace ui
{
	class View;
	typedef std::function<View*(void)> ViewConstructor;
	class ViewFactory
	{
	public:
		ViewFactory();
		~ViewFactory();

		View* CreateViewFromXml(const std::wstring& path);

		void RegisterViewClass(const std::string& name, ViewConstructor c);


	private:

		std::unordered_map<std::string, ViewConstructor> constructor_map_;
	};
}