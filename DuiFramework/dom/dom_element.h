#pragma once
#include "base/basictypes.h"
#include "dom/dom_forward.h"
#include "render/render_forward.h"

#include <set>

namespace ui
{

	class RenderObject;
	class UIElement : public RefCounted<UIElement>
	{
		friend class UIDocument;
		friend class UIElement;
		friend class RefCounted<UIElement>;
	public:
		UIElement(UIDocumentPtr);
		UIDocumentPtr GetDocument();

		UIElementPtr parent();
		bool HasChildren() const;
		bool HasParent() const;

		UIElementPtr previousSibling() const;
		UIElementPtr nextSibling() const;
		UIElementPtr firstChild() const;
		UIElementPtr lastChild() const;

		void AppendTo(UIElementPtr parent);
		void Detach();

		void Append(UIElementPtr child);
		void Prepend(UIElementPtr child);
		void Remove(UIElementPtr child);
		void InsertAfterChild(UIElementPtr ref, UIElementPtr child);
		void InsertBeforeChild(UIElementPtr ref, UIElementPtr child);

		void RemoveChildren();
		void RemoveChild(UIElementPtr n);

		virtual void AttatchRender();
		virtual void DetachRender();

		RenderObject* GetRenderObject();

		void setTag(const std::string& tag);
		const std::string& getTag() const;

		void setId(const std::string& id);
		const std::string& getId() const;

		void addClass(const std::string& v);
		void removeClass(const std::string& v);
		bool haveClass(const std::string& v) const;
		void clearClass();

		RenderBox* GetRenderBox();

		RenderStyles* GetRenderStyles();

		virtual Rect GetRenderBounds();
		virtual Rect GetContentBounds();

	protected:
		void Unlink(UIElementPtr child);
		virtual ~UIElement();

		UIDocumentPtr document_;
		
		UIElementPtr parent_{ NULL };
		UIElementPtr prev_sibling_{ NULL };
		UIElementPtr next_sibling_{ NULL };
		UIElementPtr first_child_{ NULL };
		UIElementPtr last_child_{ NULL };

		scoped_refptr<RenderObject> render_obj_;

		std::string tag_;
		std::string id_;
		std::set<std::string> classes_;

		scoped_ptr<RenderStyles> styles_;

		DISALLOW_COPY_AND_ASSIGN(UIElement);
	};
}