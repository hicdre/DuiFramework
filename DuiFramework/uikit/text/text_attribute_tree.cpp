#include "stdafx.h"
#include "text_attribute_tree.h"

namespace ui
{


	UITextAttributeTree::UITextAttributeTree(const Range& r)
		: range_(r)
	{

	}

	UITextAttributeTree::~UITextAttributeTree()
	{
		destory();
	}

	UITextAttributeDictionary* UITextAttributeTree::attributes()
	{
		if (!attributes_.get()) {
			attributes_.reset(new UITextAttributeDictionary);
		}
		return attributes_.get();
	}

	void UITextAttributeTree::destory()
	{
		for (UITextAttributeTree* item = firstSubTree_;
			item; )
		{
			UITextAttributeTree* toDelete = item;
			item = item->nextSibling_;
			delete toDelete;
		}
		firstSubTree_ = NULL;
		lastSubTree_ = NULL;
	}

	void UITextAttributeTree::unlinkSubTree(UITextAttributeTree* subTree)
	{
		if (firstSubTree_ == subTree) {
			firstSubTree_ = subTree->nextSibling_;
		}

		if (lastSubTree_ == subTree) {
			lastSubTree_ = lastSubTree_->prevSibling_;
		}

		if (subTree->prevSibling_) {
			subTree->prevSibling_->nextSibling_ = subTree->nextSibling_;
		}
		if (subTree->nextSibling_) {
			subTree->nextSibling_->prevSibling_ = subTree->prevSibling_;
		}
		subTree->superTree_ = NULL;
	}

	void UITextAttributeTree::addSubTree(UITextAttributeTree* newTree)
	{
		if (newTree->superTree_)
			newTree->superTree_->unlinkSubTree(newTree);

		if (lastSubTree_) {
			lastSubTree_->nextSibling_ = newTree;
			newTree->prevSibling_ = lastSubTree_;
			lastSubTree_ = newTree;
			newTree->nextSibling_ = NULL;
		}
		else {
			assert(firstSubTree_ == NULL);
			firstSubTree_ = lastSubTree_ = newTree;
			newTree->prevSibling_ = newTree->nextSibling_ = NULL;
		}
		newTree->superTree_ = this;
	}


	void UITextAttributeTree::preAddSubTree(UITextAttributeTree* newTree)
	{
		if (newTree->superTree_)
			newTree->superTree_->unlinkSubTree(newTree);

		if (firstSubTree_) {
			firstSubTree_->prevSibling_ = newTree;
			newTree->nextSibling_ = firstSubTree_;
			firstSubTree_ = newTree;
			newTree->prevSibling_ = NULL;
		}
		else {
			assert(lastSubTree_ == NULL);
			firstSubTree_ = lastSubTree_ = newTree;
			newTree->prevSibling_ = newTree->nextSibling_ = NULL;
		}
		newTree->superTree_ = this;
	}


	void UITextAttributeTree::removeSubTree(UITextAttributeTree* newTree)
	{
		if (!newTree || newTree->superTree_ != this) {
			assert(0);
			return;
		}

		unlinkSubTree(newTree);

		newTree->prevSibling_ = NULL;
		newTree->nextSibling_ = NULL;
	}

	void UITextAttributeTree::insertSubTreeAfter(UITextAttributeTree* newTree, UITextAttributeTree* afterTree)
	{
		assert((!afterTree || afterTree->superTree_ == this) && newTree);

		if (newTree->superTree_)
			newTree->superTree_->unlinkSubTree(newTree);

		if (afterTree == NULL) {
			preAddSubTree(newTree);
			return;
		}

		if (afterTree == lastSubTree_) {
			addSubTree(newTree);
			return;
		}

		newTree->prevSibling_ = afterTree;
		newTree->nextSibling_ = afterTree->nextSibling_;

		afterTree->nextSibling_->prevSibling_ = newTree;
		afterTree->nextSibling_ = newTree;
		newTree->superTree_ = this;
	}

	void UITextAttributeTree::insertSubTreeBefore(UITextAttributeTree* newTree, UITextAttributeTree* beforeTree)
	{
		assert((!beforeTree || beforeTree->superTree_ == this) && newTree);

		if (beforeTree == NULL) {
			addSubTree(newTree);
			return;
		}

		return insertSubTreeAfter(newTree, beforeTree->prevSibling_);
	}

	const Range& UITextAttributeTree::range() const
	{
		return range_;
	}

	UITextAttributeTree* UITextAttributeTree::findSubTreeAtIndex(size_t index)
	{
		for (UITextAttributeTree* item = firstSubTree_; item; item = item->nextSibling_)
		{
			if (item->end() <= index)
				break;
			if (item->hasIndex(index))
				return item;
		}
		return NULL;
	}

	void UITextAttributeTree::foreachSubTree(std::function<void(UITextAttributeTree* subTree, bool* isContinue)> func)
	{
		for (UITextAttributeTree* item = firstSubTree_; item; item = item->nextSibling_)
		{
			bool isContinue = true;
			func(item, &isContinue);
			if (!isContinue)
				break;
		}
	}


}