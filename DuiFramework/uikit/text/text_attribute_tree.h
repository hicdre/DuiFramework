#pragma once
#include "uikit/text/text_attribute_dictionary.h"
#include <functional>

namespace ui
{
	class UITextAttributeTree
	{
	public:
		UITextAttributeTree(const Range& r);
		~UITextAttributeTree();

		UITextAttributeDictionary* attributes();
		const Range& range() const;
		size_t start() const { return range_.start(); }
		size_t end() const { return range_.end(); }
		size_t length() const { return range_.length(); }
		bool hasIndex(size_t index) { return start() <= index && index < end(); }

		UITextAttributeTree* superTree() const { return superTree_; }
		UITextAttributeTree* nextSibling() const { return nextSibling_; }
		UITextAttributeTree* prevSibling() const { return prevSibling_; }
		UITextAttributeTree* firstSubTree() const { return firstSubTree_; }
		UITextAttributeTree* lastSubTree() const { return lastSubTree_; }

		UITextAttributeTree* findSubTreeAtIndex(size_t index);

		//modify
		
	private:
		void addSubTree(UITextAttributeTree* newTree);
		void preAddSubTree(UITextAttributeTree* newTree);
		void removeSubTree(UITextAttributeTree* newTree);
		void insertSubTreeAfter(UITextAttributeTree* newTree, UITextAttributeTree* afterTree);
		void insertSubTreeBefore(UITextAttributeTree* newTree, UITextAttributeTree* beforeTree);

		void foreachSubTree(std::function<void(UITextAttributeTree* subTree, bool* isContinue)> func);

		void destory();
		void unlinkSubTree(UITextAttributeTree* subTree);
		UITextAttributeTree* superTree_{ NULL };
		UITextAttributeTree* nextSibling_{ NULL };
		UITextAttributeTree* prevSibling_{ NULL };
		UITextAttributeTree* firstSubTree_{ NULL };
		UITextAttributeTree* lastSubTree_{ NULL };

		scoped_ptr<UITextAttributeDictionary> attributes_;
		Range range_;
	};
}