#pragma once
#include "base/basictypes.h"
#include "dom/dom_node.h"
#include <string>

namespace ui
{
	class DOMDocument : public DOMNode
	{
		friend class DOMElement;
		friend class DOMDocumentBuilder;
	public:
		DOMDocument();
		~DOMDocument();

		void Load(const std::string& str);
		void LoadFile(const std::wstring& file);

		DOMElement* RootElement();

	private:
		DISALLOW_COPY_AND_ASSIGN(DOMDocument);
	};
}
