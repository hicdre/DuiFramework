#pragma once
#include "foundation/foundation.h"

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

		void Shrink(size_t newLength, const T& val)
		{
			if (newLength >= length)
				return;
			size_t deltaLength = length - newLength;
			MyType* newNode = new MyType(deltaLength, val);
			newNode->prev = this;
			newNode->next = next;
			length = newLength;
			next = newNode;
		}

		void Expand(size_t newLength)
		{
			if (newLength <= length)
				return;
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

		UIRange(const Range& r, const T& val)
		{
			start_ = r.start();
			length_ = r.length();
			head_ = new NodeType(length_, val);
		}

		void Update(const Range& r, const T& val)
		{
			size_t dpos = 0;
			NodeType* item = FindNodeByPos(r.start(), &dpos);
			if (item == NULL)
				return;
			const T& oldVal = item->value;
			if (!item->ValueEquals(val))
			{
				item->Shrink(dpos, val);
				item = item->next;
			}

			if (item->length < r.length()) {
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
		
		NodeType* FindNodeByPos(size_t pos, size_t *dpos = NULL)
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
			return start_;
		}

		size_t end() const {
			return start_ + length_;
		}

		size_t length() const {
			return length_;
		}

		bool contains(size_t pos) const {
			return start() <= pos && pos < end();
		}
	
		EnumatorType GetEnumator() const {
			return EnumatorType(head_, start_);
		}
	private:
		NodeType* head_;
		size_t start_;
		size_t length_;

		DISALLOW_COPY_AND_ASSIGN(UIRange);
	};
}