#pragma once
#include "base\basictypes.h"

namespace ui
{
	class DOMDocument;
	class DOMNode
	{
		friend class DOMDocument;
		friend class DOMElement;
	public:
		DOMDocument* GetDocument();
		//virtual Element* ToElement() { return NULL; }

		DOMNode* parent();
		bool HasChildren() const;
		DOMNode* previousSibling() const;
		DOMNode* nextSibling() const;
		DOMNode* firstChild() const;
		DOMNode* lastChild() const;

		DOMNode* AppendTo(DOMNode* parent);
		DOMNode* Detach();

		DOMNode* Append(DOMNode* child);
		DOMNode* Remove(DOMNode* child);
		DOMNode* InsertAfter(DOMNode* ref, DOMNode* child);
		DOMNode* InsertBefore(DOMNode* ref, DOMNode* child);

		void DeleteChildren();
		void DeleteChild(DOMNode* n);

		void attach();
		void detach();
		//RenderObject* renderer() const
	protected:
		DOMNode(DOMDocument*);
		virtual ~DOMNode();
		DOMDocument* document_;
		
		DOMNode* parent_{ NULL };
		DOMNode* prev_sibling_{ NULL };
		DOMNode* next_sibling_{ NULL };
		DOMNode* first_child_{ NULL };
		DOMNode* last_child_{ NULL };

	private:
		DISALLOW_COPY_AND_ASSIGN(DOMNode);
	};
}