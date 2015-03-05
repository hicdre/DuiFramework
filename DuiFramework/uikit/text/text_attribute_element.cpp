#include "stdafx.h"
#include "text_attribute_element.h"

namespace ui
{


	UITextAttributeElement::UITextAttributeElement()
		: pos_(0)
		, length_(0)
	{

	}

	UITextAttributeElement::~UITextAttributeElement()
	{
		destory();
	}

	UITextAttributes* UITextAttributeElement::attributes()
	{
		if (!attributes_.get()) {
			attributes_.reset(new UITextAttributes);
		}
		return attributes_.get();
	}

	void UITextAttributeElement::destory()
	{
		for (UITextAttributeElement* item = firstSubElem_;
			item; )
		{
			UITextAttributeElement* toDelete = item;
			item = item->nextSibling_;
			delete toDelete;
		}
		firstSubElem_ = NULL;
		lastSubElem_ = NULL;
	}

	void UITextAttributeElement::unlinkSubTree(UITextAttributeElement* subTree)
	{
		if (firstSubElem_ == subTree) {
			firstSubElem_ = subTree->nextSibling_;
		}

		if (lastSubElem_ == subTree) {
			lastSubElem_ = lastSubElem_->prevSibling_;
		}

		if (subTree->prevSibling_) {
			subTree->prevSibling_->nextSibling_ = subTree->nextSibling_;
		}
		if (subTree->nextSibling_) {
			subTree->nextSibling_->prevSibling_ = subTree->prevSibling_;
		}
		subTree->superTree_ = NULL;
	}

	void UITextAttributeElement::addSubTree(UITextAttributeElement* newTree)
	{
		if (newTree->superTree_)
			newTree->superTree_->unlinkSubTree(newTree);

		if (lastSubElem_) {
			lastSubElem_->nextSibling_ = newTree;
			newTree->prevSibling_ = lastSubElem_;
			lastSubElem_ = newTree;
			newTree->nextSibling_ = NULL;
		}
		else {
			assert(firstSubElem_ == NULL);
			firstSubElem_ = lastSubElem_ = newTree;
			newTree->prevSibling_ = newTree->nextSibling_ = NULL;
		}
		newTree->superTree_ = this;
	}


	void UITextAttributeElement::preAddSubTree(UITextAttributeElement* newTree)
	{
		if (newTree->superTree_)
			newTree->superTree_->unlinkSubTree(newTree);

		if (firstSubElem_) {
			firstSubElem_->prevSibling_ = newTree;
			newTree->nextSibling_ = firstSubElem_;
			firstSubElem_ = newTree;
			newTree->prevSibling_ = NULL;
		}
		else {
			assert(lastSubElem_ == NULL);
			firstSubElem_ = lastSubElem_ = newTree;
			newTree->prevSibling_ = newTree->nextSibling_ = NULL;
		}
		newTree->superTree_ = this;
	}


	void UITextAttributeElement::removeSubTree(UITextAttributeElement* newTree)
	{
		if (!newTree || newTree->superTree_ != this) {
			assert(0);
			return;
		}

		unlinkSubTree(newTree);

		newTree->prevSibling_ = NULL;
		newTree->nextSibling_ = NULL;
	}

	void UITextAttributeElement::insertSubTreeAfter(UITextAttributeElement* newTree, UITextAttributeElement* afterTree)
	{
		assert((!afterTree || afterTree->superTree_ == this) && newTree);

		if (newTree->superTree_)
			newTree->superTree_->unlinkSubTree(newTree);

		if (afterTree == NULL) {
			preAddSubTree(newTree);
			return;
		}

		if (afterTree == lastSubElem_) {
			addSubTree(newTree);
			return;
		}

		newTree->prevSibling_ = afterTree;
		newTree->nextSibling_ = afterTree->nextSibling_;

		afterTree->nextSibling_->prevSibling_ = newTree;
		afterTree->nextSibling_ = newTree;
		newTree->superTree_ = this;
	}

	void UITextAttributeElement::insertSubTreeBefore(UITextAttributeElement* newTree, UITextAttributeElement* beforeTree)
	{
		assert((!beforeTree || beforeTree->superTree_ == this) && newTree);

		if (beforeTree == NULL) {
			addSubTree(newTree);
			return;
		}

		return insertSubTreeAfter(newTree, beforeTree->prevSibling_);
	}

	UITextAttributeElement* UITextAttributeElement::findElementAtIndex(size_t index)
	{
		if (!intersect(index))
			return NULL;
		index -= pos_;
		for (UITextAttributeElement* item = firstSubElem_; item; item = item->nextSibling_)
		{
			UITextAttributeElement* elem = findElementAtIndex(index);
			if (elem)
				return elem;
		}
		return this;
	}


	void UITextAttributeElement::updateAttributes(size_t pos, size_t len, UITextAttributes* attributes)
	{
		size_t end = pos + len;
		if (pos < pos_)
			pos = pos_;
		if (pos_ + length_ < end)
			end = pos_ + length_;
		if (pos >= end)
			return;
		len = pos - end;
		//todo:
// 		for (UITextAttributeElement* item = firstSubElem_; item; item = item->nextSibling_)
// 		{
// 			UITextAttributeElement* elem = findElementAtIndex(index);
// 			if (elem)
// 				return elem;
// 		}
	}

	bool UITextAttributeElement::intersect(size_t s, size_t len) const
	{
		size_t e = s + len;
		return s < end() && start() < e;
	}

	bool UITextAttributeElement::intersect(size_t pos) const
	{
		return intersect(pos, 1);
	}


}