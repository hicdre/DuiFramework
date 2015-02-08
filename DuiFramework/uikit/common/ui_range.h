#pragma once
#include "foundation/foundation.h"

namespace ui
{
// 	template <class T, class Compare>
// 	struct UIRangeNode
// 	{
// 		size_t end;//表示最后一个位置
// 		T value;
// 		UIRangeNode(size_t p, const T& val)
// 			: end(p), value(val) {}
// 		bool ValueEquals(UIRangeNode* other) const {
// 			return Compare(value, other->value);
// 		}
// 
// 	};

	template <class T, class Compare = std::equal_to<T>>
	class UIRange
	{
	public:
		struct Node
		{
			size_t pos;
			size_t length;
			T value;
			Node(size_t p, size_t l, const T& val)
				: pos(p), length(l), value(val) {}
			bool ValueEquals(Node* other) const {
				return Compare()(value, other->value);
			}
			bool ValueEquals(const T& val) const {
				return Compare()(value, val);
			}

			size_t end() const {
				return pos + length;
			}
		};
		typedef Node NodeType;
		typedef std::vector<NodeType> ContainerType;

		UIRange(const Range& r, const T& val)
		{
			offset_ = r.start();
			container_.push_back(Node(0, r.length(), val));
		}

		void Update(const Range& r, const T& val)
		{
			Range localRange(r.start() - offset_, r.end() - offset_);
			if (localRange.end() > maxLength()) {
				localRange.set_end(maxLength());
			}

			int index = findRangeNodeIndexWithPos(localRange.start());

			{
				const Node& node = container_[index];
				if (!node.ValueEquals(val))
				{
					if (node.pos != localRange.start())
					{
						container_[index].length = localRange.start() - node.pos;
						container_.insert(container_.begin() + index + 1, Node(localRange.start(), localRange.length(), val));
						index++;
					}
					else
					{
						container_[index].value = val;
					}
				}
				//index指向范围起点位置
			}

			if (index == maxIndex())
				return;

			int endIndex = findRangeNodeIndexWithPos(index + 1, maxIndex(), localRange.end() - 1);
			{
				const Node& node = container_[endIndex];
				container_[endIndex].length -= localRange.end() - node.pos;
				container_[endIndex].pos = localRange.end();
			}

			container_.erase(container_.begin() + index + 1, container_.begin() + endIndex);

		}

	private:
		size_t maxLength() const {
			return container_[maxIndex()].end();
		}

		int maxIndex() const {
			return container_.size() - 1;
		}

		size_t getEndPostAtIndex(int i) const {
			return container_[i].end();
		}

		size_t getStartPostAtIndex(int i) const {
			return container_[i].pos;
		}

		int findRangeNodeIndexWithPos(size_t start, size_t end, size_t p) const {
			if (p < maxLength())
			{
				//折半查找
				while (start < end)
				{
					size_t mid = (start + end) / 2;
					const Node& node = container_[mid];
					if (node.pos <= p && p < node.end())
						return mid;
					else if (node.end() <= p)
						start = mid + 1;
					else
						end = mid - 1;
				}
				return start;
			}
			return -1;
		}

		int findRangeNodeIndexWithPos(size_t p) const {
			return findRangeNodeIndexWithPos(0, maxIndex(), p);
		}

		ContainerType container_;
		int offset_;
	};
}