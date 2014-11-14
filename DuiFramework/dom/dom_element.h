#pragma once
#include "base/basictypes.h"
#include "dom/dom_node.h"
#include "render/render_object.h"
#include <set>

namespace ui
{
	class DOMElement : public DOMNode
	{
		friend class DOMDocument;
	public:
		DOMElement(DOMDocument* doc);
		~DOMElement();

		virtual const char* getTag() const;

		void setId(const std::string& id);
		const std::string& getId() const;

		void addClass(const std::string& v);
		void removeClass(const std::string& v);
		bool haveClass(const std::string& v) const;
		void clearClass();



	protected:
		std::string id_;
		std::set<std::string> classes_;
		RenderObject* render_tree_;
	};
}