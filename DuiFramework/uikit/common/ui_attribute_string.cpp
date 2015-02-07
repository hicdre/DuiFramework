#include "stdafx.h"
#include "ui_attribute_string.h"

namespace ui
{

	UICharAttributeRangeList::UICharAttributeRangeList(const Range& r)
	{
		PushBack(new UICharAttributeRange(r));
	}

	UICharAttributeRangeList::UICharAttributeRangeList()
		: first(NULL)
		, last(NULL)
	{

	}

	UICharAttributeRangeList::~UICharAttributeRangeList()
	{
		Clear();
	}

	UICharAttributeRange* UICharAttributeRangeList::PushBack(UICharAttributeRange* newRange)
	{
		return InsertAfter(last, newRange);
	}

	UICharAttributeRange* UICharAttributeRangeList::InsertAfter(
		UICharAttributeRange* prevRange, 
		UICharAttributeRange* newRange)
	{
		newRange->prev = prevRange;

		if (prevRange)
		{
			newRange->next = prevRange->next;
			if (prevRange->next)
				prevRange->next->prev = newRange;
			else
				last = prevRange;
			prevRange->next = newRange;
		}
		else
		{
			newRange->next = first;
			if (first)
				first->prev = newRange;
			else
				last = newRange;

			first = newRange;
		}
		return newRange;
	}

	UICharAttributeRange* UICharAttributeRangeList::Remove(UICharAttributeRange* deleteRange)
	{
		if (deleteRange->prev)
			deleteRange->prev->next = deleteRange->next;
		else
			first = deleteRange->next;

		if (deleteRange->next)
			deleteRange->next->prev = deleteRange->prev;
		else
			last = deleteRange->prev;

		UICharAttributeRange* ret = deleteRange->prev;
		delete deleteRange;
		return ret;
	}

	void UICharAttributeRangeList::Clear()
	{
		UICharAttributeRange* item = last;
		while (item)
		{
			UICharAttributeRange* p = item;
			item = item->prev;
			delete p;
		}
		first = NULL;
		last = NULL;
	}

	void UICharAttributeRangeList::Build()
	{
		//合并相邻属性相同区段
		UICharAttributeRange* merge_begin = first;
		size_t merge_count = 0;
		for (UICharAttributeRange* p = merge_begin->next; p != NULL; p = p->next)
		{//直接向后合并
			if (!p->attr.Equals(merge_begin->attr))
			{
				if (merge_count)
				{
					size_t begin = merge_begin->range.start();
					size_t end = p->prev->range.end();

					UICharAttributeRange* q = merge_begin->next;
					while (merge_count)
					{
						UICharAttributeRange* to_delete = q;
						q = q->next;
						Remove(to_delete);
						merge_count--;
					}

					assert(merge_begin->next == p);
					merge_begin->range.set_end(p->range.start());
				}

				merge_begin = p;
				merge_count = 0;
			}
			else
			{
				merge_count++;
			}
		}
	}

	UICharAttribute* UICharAttributeRangeList::GetAttributeAtIndex(size_t index)
	{
		return GetAttributeAtRange(Range(index, index + 1));
	}

	UICharAttribute* UICharAttributeRangeList::GetAttributeAtRange(const Range& r)
	{
		for (UICharAttributeRange* item = first; item != NULL; item = item->next)
		{
			Range range = item->range;
			//需要完全包含
			if (range.Contains(r))
			{
				return &item->attr;
			}
		}
		return NULL;
	}

	void UICharAttributeRangeList::ForIn(std::function<bool(const Range&, const UICharAttribute&)> callback) const
	{
		for (UICharAttributeRange* item = first; item != NULL; item = item->next)
		{
			if (!callback(item->range, item->attr))
				break;
		}
	}

	void UICharAttributeRangeList::UpdateAttributeInRange(const Range& r, std::function<void(UICharAttribute&)> callback)
	{
		for (UICharAttributeRange* item = first; item != NULL; item = item->next)
		{
			Range range = item->range;
			//需要完全包含
			if (r.Contains(range))
			{
				callback(item->attr);
			}
		}
	}

	void UICharAttributeRangeList::UpdateAttributeBreakRange(const Range& r, std::function<void(UICharAttribute&)> callback)
	{
		for (UICharAttributeRange* item = first; item != NULL; item = item->next)
		{
			Range range = item->range;
			//需要完全包含
			if (r.Contains(range))
			{
				callback(item->attr);
				if (r.end() == range.end())
					break;
			}
			else if (range.start() < r.start()
				&& r.start() < range.end()
				&& range.end() <= r.end())
			{//前部分相交
				UICharAttributeRange* newRange = new UICharAttributeRange(Range(r.start(), range.end()));
				item->range.set_end(r.start());
				item = InsertAfter(item, newRange);
				callback(newRange->attr);
			}
			else if (r.start() < range.start()
				&& range.start() < r.end()
				&& r.end() <= range.end())
			{//后部分相交
				UICharAttributeRange* newRange = new UICharAttributeRange(Range(range.start(), r.end()));
				item->range.set_start(r.end());
				InsertAfter(item->prev, newRange);
				callback(newRange->attr);
				break;
			}
			else if (range.Contains(r))
			{
				UICharAttributeRange* newRange1 = new UICharAttributeRange(r);
				UICharAttributeRange* newRange2 = new UICharAttributeRange(Range(r.end(), range.end()));
				item->range.set_end(r.start());
				item = InsertAfter(item, newRange1);
				item = InsertAfter(item, newRange2);
				callback(newRange1->attr);
				break;
			}
		}
		Build();
	}

	Range UICharAttributeRangeList::GetRangeWithSameAttribute(size_t index) const
	{
		Range r(index, index + 1);
		for (const UICharAttributeRange* item = first; item != NULL; item = item->next)
		{
			const Range& range = item->range;
			//需要完全包含
			if (range.Contains(r))
			{
				return item->range;
			}
		}
		return Range::InvalidRange();
	}

	UICharAttributeRangeList* UICharAttributeRangeList::Copy(const Range& r) const
	{
		if (r.length() == 0)
			return NULL;
		UICharAttributeRangeList* fragment = new UICharAttributeRangeList;

		UICharAttributeRange* insert_after = NULL;
		for (UICharAttributeRange* item = first; item != NULL; item = item->next)
		{
			Range key = item->range;
			if (key.end() <= r.start())
			{//未到起始位置
				continue;
			}
			else if (r.start() <= key.start() && key.end() <= r.end())
			{//包含
				insert_after = fragment->InsertAfter(insert_after,
					new UICharAttributeRange(Range(key.start(), key.end()), item->attr));
			}
			else if (r.start() < key.start() 
				&& key.start() < r.end() 
				&& r.end() < key.end())
			{//右交, 到达右边界
				insert_after = fragment->InsertAfter(insert_after,
					new UICharAttributeRange(Range(key.start(), r.end()), item->attr));
				break;
			}
			else if (key.start() <= r.start() && r.end() < key.end())
			{//被包含, 到达右边界
				insert_after = fragment->InsertAfter(insert_after,
					new UICharAttributeRange(r, item->attr));
				break;
			}
			else if (key.start() < r.start() 
				&& r.start() < key.end() 
				&& key.end() < r.end())
			{//左交
				insert_after = fragment->InsertAfter(insert_after,
					new UICharAttributeRange(Range(r.start(), key.end()), item->attr));
			}
		}
		return fragment;
	}

	void UICharAttributeRangeList::Normalize()
	{
		Offset(-(int32)first->range.start());
	}

	void UICharAttributeRangeList::Offset(int32 offset)
	{
		for (UICharAttributeRange* item = first; item != NULL; item = item->next)
		{
			Range range = item->range;
			range.set_start(range.start() + offset);
			range.set_end(range.end() + offset);
		}
	}

	
	UIAttributeString::UIAttributeString()
	{

	}
}