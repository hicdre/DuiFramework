#pragma once
#include "uikit/text/text_attribute_dictionary.h"
#include <functional>

namespace ui
{
	class UITextAttributeElement
	{
	public:
		UITextAttributeElement();
		~UITextAttributeElement();

		UITextAttributes* attributes();
		size_t start() const { return pos_; }
		size_t end() const { return pos_ + length_; }
		size_t length() const { return length_; }

		bool needRemove() const { return length_ == 0; }

		UITextAttributeElement* superElement() const { return superTree_; }
		UITextAttributeElement* nextSibling() const { return nextSibling_; }
		UITextAttributeElement* prevSibling() const { return prevSibling_; }
		UITextAttributeElement* firstSubTree() const { return firstSubElem_; }
		UITextAttributeElement* lastSubTree() const { return lastSubElem_; }

		//
		size_t convertIndexToSuper(size_t index) const { return index + pos_; }
		size_t convertIndexFromSuper(size_t index) const { return index - pos_; }
		UITextAttributeElement* findElementAtIndex(size_t index);

		//modify
		void updateAttributes(size_t pos, size_t len, UITextAttributes* attributes);
		
	private:
		bool intersect(size_t pos, size_t len) const;
		bool intersect(size_t pos) const;
		void addSubTree(UITextAttributeElement* newTree);
		void preAddSubTree(UITextAttributeElement* newTree);
		void removeSubTree(UITextAttributeElement* newTree);
		void insertSubTreeAfter(UITextAttributeElement* newTree, UITextAttributeElement* afterTree);
		void insertSubTreeBefore(UITextAttributeElement* newTree, UITextAttributeElement* beforeTree);

		void destory();
		void unlinkSubTree(UITextAttributeElement* subTree);
		UITextAttributeElement* superTree_{ NULL };
		UITextAttributeElement* nextSibling_{ NULL };
		UITextAttributeElement* prevSibling_{ NULL };
		UITextAttributeElement* firstSubElem_{ NULL };
		UITextAttributeElement* lastSubElem_{ NULL };

		scoped_ptr<UITextAttributes> attributes_;
		size_t pos_;//在父elem中位置
		size_t length_;//elem长度
	};
}