#pragma once
#include "base/basictypes.h"
#include "dom/dom_forward.h"

namespace ui
{

	class RenderObject;
	class UINode : public RefCounted<UINode>
	{
		friend class UIDocument;
		friend class UIElement;
		friend class RefCounted<UINode>;
	public:
		UIDocumentPtr GetDocument();

		UINodePtr parent();
		bool HasChildren() const;
		bool HasParent() const;

		UINodePtr previousSibling() const;
		UINodePtr nextSibling() const;
		UINodePtr firstChild() const;
		UINodePtr lastChild() const;

		void AppendTo(UINodePtr parent);
		void Detach();

		void Append(UINodePtr child);
		void Prepend(UINodePtr child);
		void Remove(UINodePtr child);
		void InsertAfterChild(UINodePtr ref, UINodePtr child);
		void InsertBeforeChild(UINodePtr ref, UINodePtr child);

		void RemoveChildren();
		void RemoveChild(UINodePtr n);

		virtual void AttatchRender();
		virtual void DetachRender();

		RenderObject* GetRenderObject();

		virtual Rect GetRenderBounds();
		virtual Rect GetContentBounds();

	protected:
		void Unlink(UINodePtr child);
		
		UINode(UIDocumentPtr);
		virtual ~UINode();

		UIDocumentPtr document_;
		
		UINodePtr parent_{ NULL };
		UINodePtr prev_sibling_{ NULL };
		UINodePtr next_sibling_{ NULL };
		UINodePtr first_child_{ NULL };
		UINodePtr last_child_{ NULL };

		scoped_refptr<RenderObject> render_obj_;

		DISALLOW_COPY_AND_ASSIGN(UINode);
	};
}