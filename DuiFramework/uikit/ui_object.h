#pragma once
#include "foundation/foundation.h"
#include <functional>
#include <unordered_map>

namespace ui
{
	typedef std::string Selector;
	class UIObject
	{
	public:
		typedef std::function<void(void)> FunctionType;
		virtual ~UIObject();

		void registerSelector(const Selector& s, FunctionType func);
		void performSelector(const Selector& s);
	private:
		std::unordered_map<Selector, FunctionType> function_map_;
	};
}