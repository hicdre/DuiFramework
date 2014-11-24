#pragma once
#include "base/basictypes.h"
#include "dom/dom_node.h"
#include "render/render_object.h"
#include <set>

namespace ui
{
	class UIElement : public UINode
	{
		friend class UIDocument;
	public:
		UIElement(UIDocumentPtr doc);
		~UIElement();

		void setTag(const std::string& tag);
		const std::string& getTag() const;

		void setId(const std::string& id);
		const std::string& getId() const;

		void addClass(const std::string& v);
		void removeClass(const std::string& v);
		bool haveClass(const std::string& v) const;
		void clearClass();



	protected:
		std::string tag_;
		std::string id_;
		std::set<std::string> classes_;
		RenderObject* render_tree_;
	};
}