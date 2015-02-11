#pragma once
#include "foundation/foundation.h"
#include <functional>

namespace ui
{
	template <class T, class Compare = std::equal_to<T>>
	struct UIRangeNode
	{
		typedef UIRangeNode<T, Compare> MyType;
		MyType* prev{ NULL };
		MyType* next{ NULL };
		size_t length;
		T value;
		UIRangeNode(size_t l, const T& val)
			: length(l), value(val) {}
		bool ValueEquals(MyType* other) const {
			return Compare()(value, other->value);
		}
		bool ValueEquals(const T& val) const {
			return Compare()(value, val);
		}
		bool IsEnd() const { return next == NULL; }
		bool IsBegin() const { return prev == NULL; }

		UIRangeNode* Copy() const {
			return new UIRangeNode(length, value);
		}

		void Shrink(size_t newLength, const T& val)
		{
			if (newLength >= length)
				return;
			assert(newLength);
			size_t deltaLength = length - newLength;
			MyType* newNode = new MyType(deltaLength, val);
			newNode->prev = this;
			newNode->next = next;
			length = newLength;
			next = newNode;
		}

		void Expand(size_t newLength)
		{
			if (newLength < length) {
				assert(0);
				return;
			}
				
			if (newLength == length && next && ValueEquals(next)) {
				MyType* node = next;
				length += node->length;
				if (node->next)
					node->next->prev = this;
				next = node->next;
				delete node;
				return;
			}

			size_t deltaLength = newLength - length;
			
			MyType* node = next;
			while (node && node->length <= deltaLength)
			{
				MyType* to_del = node;
				deltaLength -= to_del->length;
				node = node->next;
				delete to_del;
			}

			if (node && node->length > deltaLength) {
				if (!ValueEquals(node)) {
					node->length -= deltaLength;
					next = node;
					length = newLength;
				}
				else {
					length = newLength + node->length - deltaLength;
					next = node->next;
					delete node;
				}
			}
			else {
				length = newLength;
			}

		}
	};

	template <class T, class Compare = std::equal_to<T>>
	class UIRangeEnumator
	{
	public:
		typedef UIRangeNode<T, Compare> NodeType;
		UIRangeEnumator(NodeType* n, size_t p)
			: node(n), pos(p) {
		}
		Range range() const {
			return Range(pos, node->length);
		}
		const T& value() const {
			return node->value;
		}
		bool isVaild() const {
			return !!node;
		}
		void next() {
			pos = pos + node->length;
			node = node->next;
		}

	private:
		NodeType* node;
		size_t pos;
	};

	template <class T, class Compare = std::equal_to<T>>
	class UIRange
	{
	public:
		typedef UIRangeNode<T, Compare> NodeType;
		typedef UIRangeEnumator<T, Compare> EnumatorType;

		UIRange() {
			head_ = NULL;
			start_ = 0;
			length_ = 0;
		}

		UIRange(const Range& r, const T& val)
		{
			start_ = r.start();
			length_ = r.length();
			if (r.length())
				head_ = new NodeType(length_, val);
		}

		UIRange(size_t length, const T& val)
		{
			start_ = 0;
			length_ = length;
			if (length)
				head_ = new NodeType(length_, val);
		}

		void normalize() {
			start_ = 0;
		}

		void Update(const Range& r, const T& val)
		{
			if (head_ == NULL) {
				start_ = r.start();
				length_ = r.length();
				head_ = new NodeType(length_, val);
				return;
			}
			size_t dpos = 0;
			NodeType* item = FindNodeByPos(r.start(), &dpos);
			if (item == NULL)
				return;
			const T& oldVal = item->value;
			if (!item->ValueEquals(val))
			{
				if (dpos != 0) {
					item->Shrink(dpos, val);
					item = item->next;
				}
				else {
					if (item->prev && item->prev->ValueEquals(val)) {
						item->prev->Expand(item->prev->length + r.length());
						return;
					}
					else {
						item->value = val;
					}
					
				}
			}

			if (item->length > r.length()) {
				item->Shrink(r.length(), oldVal);
			}
			else {
				item->Expand(r.length());
			}
			
		}

		void Insert(size_t pos, size_t num) {
			NodeType* item = FindNodeByPos(pos);
			if (!item)
				return;
			item->length += num;
			length_ += num;
		}

		//todo:fix
		void Erase(size_t pos, size_t num) {
			size_t dpos = 0;
			NodeType* item = FindNodeByPos(pos, &dpos);
			if (!item)
				return;
			if (item->length >= dpos + num) {
				item->length -= num;
			}
			else{
				num = item->length - dpos;
				item->length = dpos;
				item = item->next;
				while (item && item->length <= num) {
					MyType* to_del = item;
					num -= to_del->length;
					item = item->next;
					delete to_del;
				}

				if (item && item->length > num) {
					if (!ValueEquals(node)) {
						item->length -= num;
					}
					else {
						item->prev->length += item->length - num;
						item->prev->next = item->next;
						delete item;
					}
				}
			}
		}
		
		UIRange* Copy(const Range& r) const
		{
			size_t dpos = 0;
			NodeType* item = FindNodeByPos(r.start(), &dpos);
			if (item == NULL)
				return NULL;

			size_t deltaLength = r.length();

			NodeType* newNodeHead = item->Copy();
			NodeType* newNode = newNodeHead;
			{
				if (newNode->length >= dpos + r.length()) {
					newNode = r.length();
					return new UIRange(newNodeHead, r.start(), r.length());
				}
				else {
					deltaLength -= newNode->length - dpos;
					newNode->length -= dpos;
				}
			}
			
			item = item->next;
			while (deltaLength && item && item->length <= deltaLength)
			{
				newNode->next = item->Copy();
				newNode->next->prev = newNode;
				newNode = newNode->next;

				if (item->length > deltaLength) {
					newNode->length = deltaLength;
					break;
				}
				else {
					deltaLength -= item->length;
				}
				
				item = item->next;
			}

			return new UIRange(newNodeHead, r.start(), r.length());
		}

		NodeType* FindNodeByPos(size_t pos, size_t *dpos = NULL) const
		{
			if (!contains(pos))
				return NULL;
			size_t distance = pos - start();
			NodeType* item = head_;
			while (item && item->length <= distance) {
				distance -= item->length;
				item = item->next;
			}
			if (item && dpos) {
				*dpos = distance;
			}
			return item;
		}

		size_t start() const {
			if (head_ == NULL)
				return 0;
			return start_;
		}

		size_t end() const {
			if (head_ == NULL)
				return 0;
			return start_ + length_;
		}

		size_t length() const {
			if (head_ == NULL)
				return 0;
			return length_;
		}

		bool isEmpty() const {
			return !!head_;
		}

		bool contains(size_t pos) const {
			return start() <= pos && pos < end();
		}
	
		EnumatorType GetEnumator() const {
			return EnumatorType(head_, start_);
		}
	
		std::string rangeDescription(std::function<std::string(const T& val)> printFunc) const {
			if (head_ == NULL)
				return "null";
			NodeType* item = head_;
			std::string outString;
			size_t start = start_;
			while (item) {
				size_t end = start + item->length;
				if (item != head_)
					outString += ",";
				StringAppendF(&outString, "[%d,%d)=>%s", start, end, printFunc(item->value).c_str());
				start = end;
				item = item->next;
			}
			return outString;
		}
	private:
		UIRange(NodeType* head, size_t start, size_t length)
			: head_(head), start_(start), length_(length) {}
		NodeType* head_;
		size_t start_;
		size_t length_;

		DISALLOW_COPY_AND_ASSIGN(UIRange);
	};
}