#pragma once
#include "base/basictypes.h"
#include "dom/dom_node.h"

namespace ui
{
	class DOMElement : public DOMNode
	{
		friend class DOMDocument;
	public:
		DOMElement();
		~DOMElement();

		void setTag(const std::string& tag);
		std::string getTag();


	protected:
		std::string tag_;
	};
}