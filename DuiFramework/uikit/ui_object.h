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
		typedef std::function<void(UIObject*, uint32)> FunctionType;
		virtual ~UIObject();

		void registerSelector(const Selector& s, FunctionType func);
		void performSelector(const Selector& s);
		void performSelectorWithTargetAndEvent(const Selector& s, UIObject* target, uint32 e);
	private:
		std::unordered_map<Selector, FunctionType> function_map_;
	};
}