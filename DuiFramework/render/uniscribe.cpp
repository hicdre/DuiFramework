#include "stdafx.h"
#include "uniscribe.h"

namespace ui
{
	namespace uniscribe
	{

		bool UColor::operator!=(const UColor& other) const
		{
			return fontColor != other.fontColor || backgroundColor != other.backgroundColor;
		}

		bool UColor::operator==(const UColor& other) const
		{
			return fontColor == other.fontColor && backgroundColor == other.backgroundColor;
		}


		ColorRangeItem::ColorRangeItem(size_t begin, size_t end, UColor c) : range(begin, end), color(c)
		{

		}

		ColorRangeItem::ColorRangeItem(size_t length) : range(0, length)
		{

		}


		void ColorRangeList::Clear()
		{
			ColorRangeItem* item = last;
			while (item)
			{
				ColorRangeItem* p = item;
				item = item->prev;
				delete p;
			}
			first = NULL;
			last = NULL;
		}

		ColorRangeItem* ColorRangeList::Remove(ColorRangeItem* p)
		{
			if (p->prev)
				p->prev->next = p->next;
			else
				first = p->next;

			if (p->next)
				p->next->prev = p->prev;
			else
				last = p->prev;

			ColorRangeItem* ret = p->prev;
			delete p;
			return ret;
		}

		ColorRangeItem* ColorRangeList::InsertAfter(ColorRangeItem* p, size_t start, size_t end, const UColor& c)
		{
			ColorRangeItem* new_item = new ColorRangeItem(start, end, c);
			new_item->prev = p;

			if (p)
			{
				new_item->next = p->next;
				if (p->next)
					p->next->prev = new_item;
				else
					last = p;
				p->next = new_item;
			}
			else
			{
				new_item->next = first;
				if (first)
					first->prev = new_item;
				else
					last = new_item;

				first = new_item;
			}
			return new_item;
		}

		ColorRangeItem* ColorRangeList::PushBack(size_t start, size_t end, const UColor& c)
		{
			return InsertAfter(last, start, end, c);
		}

		ColorRangeList::~ColorRangeList()
		{
			Clear();
		}


		Fragment* Fragment::Copy(size_t start, size_t length)
		{
			Fragment* fragment = new Fragment(text.substr(start, length));
			fragment->font = font;

			if (length)
			{
				fragment->colors.Clear();
				ColorRangeItem* insert_after = NULL;
				//CutColors(start, length);//todo:此逻辑是否必要
				size_t end = start + length;
				for (ColorRangeItem* item = colors.first; item != NULL; item = item->next)
				{
					Range key = item->range;
					if (start <= key.start() && key.end() <= end)
					{//包含
						insert_after = fragment->colors.InsertAfter(insert_after,
							key.start() - start, key.end() - start, item->color);
					}
					else if (start < key.start() && key.start() < end && end < key.end())
					{//左交, 到达右边界
						insert_after = fragment->colors.InsertAfter(insert_after,
							key.start() - start, end - start, item->color);
						break;
					}
					else if (key.start() <= start && end < key.end())
					{//被包含, 到达右边界
						insert_after = fragment->colors.InsertAfter(insert_after,
							0, end - start, item->color);
						break;
					}
					else if (key.start() < start && start < key.end() && key.end() < end)
					{//右交
						insert_after = fragment->colors.InsertAfter(insert_after,
							0, key.end() - start, item->color);
					}
				}
			}
			return fragment;
		}

		ui::uniscribe::UColor Fragment::GetCharColor(size_t index)
		{
			for (ColorRangeItem* item = colors.first; item != NULL; item = item->next)
			{
				Range key = item->range;
				if (key.start() <= index && index < key.end())
				{
					return item->color;
				}
			}
			return UColor();
		}

		void Fragment::DefragmentColors()
		{
			//合并相邻颜色相同区段
			ColorRangeItem* merge_begin = colors.first;
			size_t merge_count = 0;
			for (ColorRangeItem* p = merge_begin->next; p != NULL; p = p->next)
			{//直接向后合并
				if (p->color != merge_begin->color)
				{
					if (merge_count)
					{
						size_t begin = merge_begin->range.start();
						size_t end = p->range.end();

						ColorRangeItem* q = merge_begin->next;
						while (merge_count)
						{
							ColorRangeItem* to_delete = q;
							q = q->next;
							colors.Remove(to_delete);
							merge_count--;
						}

						colors.InsertAfter(merge_begin, begin, end, merge_begin->color);

						colors.Remove(merge_begin);
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

		void Fragment::UpdateBackgroundColor(size_t start, size_t length, Color color)
		{
			size_t end = start + length;
			for (ColorRangeItem* item = colors.first; item != NULL; item = item->next)
			{
				Range key = item->range;
				if (key.start() < end && start < key.end())
				{
					item->color.backgroundColor = color;
				}
			}
		}

		void Fragment::UpdateFontColor(size_t start, size_t length, Color color)
		{
			size_t end = start + length;
			for (ColorRangeItem* item = colors.first; item != NULL; item = item->next)
			{
				Range key = item->range;
				if (key.start() < end && start < key.end())
				{
					item->color.fontColor = color;
				}
			}
		}

		void Fragment::CutColors(size_t start, size_t length)
		{
			size_t end = start + length;

			for (ColorRangeItem* item = colors.first; item != NULL;)
			{
				Range key = item->range;
				if (key.start() < end && start < key.end())
				{//有交集

					size_t s1 = key.start();
					size_t s2 = key.start() > start ? key.start() : start;
					size_t s3 = key.end() < end ? key.end() : end;
					size_t s4 = key.end();

					UColor c = item->color;
					item = colors.Remove(item);

					if (s1 < s2)
					{
						item = colors.InsertAfter(item, s1, s2, c);
					}
					if (s2 < s3)
					{
						item = colors.InsertAfter(item, s2, s3, c);
					}
					if (s3 < s4)
					{
						item = colors.InsertAfter(item, s3, s4, c);
					}
					assert(item);
				}
				item = item->next;
			}
		}

		Fragment::Fragment(const std::wstring& t) : text(t)
		{
			colors.PushBack(0, t.size(), UColor());
		}

	}
}