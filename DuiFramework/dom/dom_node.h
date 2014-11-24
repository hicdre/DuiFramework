#pragma once
#include "base\basictypes.h"

namespace ui
{
	class UIDocument;
	class UINode;
	class UIElement;
	typedef scoped_refptr<UIDocument> UIDocumentPtr;
	typedef scoped_refptr<UINode> UINodePtr;
	typedef scoped_refptr<UIElement> UIElementPtr;

	class UINode : public RefCounted<UINode>
	{
		friend class UIDocument;
		friend class UIElement;
		friend class RefCounted<UINode>;
	public:
		UIDocumentPtr GetDocument();

		UINodePtr parent();
		bool HasChildren() const;

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

	private:
		DISALLOW_COPY_AND_ASSIGN(UINode);
	};
}