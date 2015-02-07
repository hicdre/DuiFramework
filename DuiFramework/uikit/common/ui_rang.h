#pragma once
#include "foundation/foundation.h"

namespace ui
{
	template <class T, class Compare = std::equal_to<T>>
	struct UIRangeNode
	{
		UIRangeNode* prev{ NULL };
		UIRangeNode* next{ NULL };
		size_t pos;
		T value;
		UIRangeNode(size_t p, const T& val)
			: pos(p), value(val) {}
		bool Equals(UIRangeNode* other) const {
			return Compare(value, other->value);
		}
		bool IsEnd() const { return next == NULL; }
		bool IsBegin() const { return prev == NULL; }

		Range range() const {
			assert(next);
			assert(next->pos > pos);
			return Range(pos, next->pos);
		}

		void Spilt(size_t midpos, T val)
		{
			assert(next);
			NodeType* newNode = new NodeType(midpos, val);
			newNode->prev = this;
			newNode->next = next;
			next = newNode;
		}

		void MergeTo(size_t endpos)
		{
			if (endpos <= pos)
				return;
			NodeType* node = next;
			while (node->next && node->next->pos <= endpos)
			{
				NodeType* to_del = node;
				node = node->next;
				delete to_del;
			}

			if (node->next == NULL) {
				next = node->next;
			} else if (node->next->pos > endpos) {
				node->pos = endpos;
				next = node;
			}
		}
	};

	template <class T, class Compare = std::equal_to<T>>
	class UIRange
	{
	public:
		typedef UIRangeNode<T, Compare> NodeType;

		UIRange(const Range& r, const T& val)
		{
			first_ = new NodeType;
			first_->pos = r.start();
			first_->value = val;

			last_ = new NodeType;
			last_->pos = r.end();
			last_->value = val;//以后不再改变
		}

		void Update(const Range& r, const T& val)
		{
			NodeType* item = first_;
			while (item != last_ && item->next->pos <= r.start())
				item = item->next;
			NodeType* originItem = item;
			item->Spilt(r.start(), val);
			item = item->next;
			if (item->next->pos < r.end())
				item->MergeTo(r.end());
			else if (item->next->pos > r.end())
				item->Spilt(r.end(), originItem->value);
		}

	private:
		NodeType* first_;
		NodeType* last_;
	};
}