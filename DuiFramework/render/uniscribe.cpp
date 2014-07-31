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


		UColorRangeItem::UColorRangeItem(size_t begin, size_t end, UColor c) : range(begin, end), color(c)
		{

		}

		UColorRangeItem::UColorRangeItem(size_t length) : range(0, length)
		{

		}


		void UColorRangeList::Clear()
		{
			UColorRangeItem* item = last;
			while (item)
			{
				UColorRangeItem* p = item;
				item = item->prev;
				delete p;
			}
			first = NULL;
			last = NULL;
		}

		UColorRangeItem* UColorRangeList::Remove(UColorRangeItem* p)
		{
			if (p->prev)
				p->prev->next = p->next;
			else
				first = p->next;

			if (p->next)
				p->next->prev = p->prev;
			else
				last = p->prev;

			UColorRangeItem* ret = p->prev;
			delete p;
			return ret;
		}

		UColorRangeItem* UColorRangeList::InsertAfter(UColorRangeItem* p, size_t start, size_t end, const UColor& c)
		{
			UColorRangeItem* new_item = new UColorRangeItem(start, end, c);
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

		UColorRangeItem* UColorRangeList::PushBack(size_t start, size_t end, const UColor& c)
		{
			return InsertAfter(last, start, end, c);
		}

		UColorRangeList::~UColorRangeList()
		{
			Clear();
		}


		UFragment* UFragment::Copy(size_t start, size_t length)
		{
			UFragment* fragment = new UFragment(text.substr(start, length));
			fragment->font = font;

			if (length)
			{
				fragment->colors.Clear();
				UColorRangeItem* insert_after = NULL;
				//CutColors(start, length);//todo:此逻辑是否必要
				size_t end = start + length;
				for (UColorRangeItem* item = colors.first; item != NULL; item = item->next)
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

		ui::uniscribe::UColor UFragment::GetCharColor(size_t index)
		{
			for (UColorRangeItem* item = colors.first; item != NULL; item = item->next)
			{
				Range key = item->range;
				if (key.start() <= index && index < key.end())
				{
					return item->color;
				}
			}
			return UColor();
		}

		void UFragment::DefragmentColors()
		{
			//合并相邻颜色相同区段
			UColorRangeItem* merge_begin = colors.first;
			size_t merge_count = 0;
			for (UColorRangeItem* p = merge_begin->next; p != NULL; p = p->next)
			{//直接向后合并
				if (p->color != merge_begin->color)
				{
					if (merge_count)
					{
						size_t begin = merge_begin->range.start();
						size_t end = p->range.end();

						UColorRangeItem* q = merge_begin->next;
						while (merge_count)
						{
							UColorRangeItem* to_delete = q;
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

		void UFragment::UpdateBackgroundColor(size_t start, size_t length, Color color)
		{
			size_t end = start + length;
			for (UColorRangeItem* item = colors.first; item != NULL; item = item->next)
			{
				Range key = item->range;
				if (key.start() < end && start < key.end())
				{
					item->color.backgroundColor = color;
				}
			}
		}

		void UFragment::UpdateFontColor(size_t start, size_t length, Color color)
		{
			size_t end = start + length;
			for (UColorRangeItem* item = colors.first; item != NULL; item = item->next)
			{
				Range key = item->range;
				if (key.start() < end && start < key.end())
				{
					item->color.fontColor = color;
				}
			}
		}

		void UFragment::CutColors(size_t start, size_t length)
		{
			size_t end = start + length;

			for (UColorRangeItem* item = colors.first; item != NULL;)
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

		UFragment::UFragment(const std::wstring& t) : text(t)
		{
			colors.PushBack(0, t.size(), UColor());
		}


		void UGlyphData::BuildData(HDC dc, SCRIPT_ITEM* scriptItem, SCRIPT_LOGATTR* charLogattrs, const wchar_t* runText, size_t length)
		{
			size_t glyphCount = 0;
			for (size_t i = 0; i < length; i++)
			{
				if (i == 0 || charLogattrs[i].fCharStop)
					glyphCount++;
			}

			ClearData(glyphCount, length);
			sa = scriptItem->a;
			memset(&glyphs[0], 0, sizeof(glyphs[0])*glyphs.size());
			memset(&glyphVisattrs[0], 0, sizeof(glyphVisattrs[0])*glyphVisattrs.size());
			memset(&glyphAdvances[0], 0, sizeof(glyphAdvances[0])*glyphAdvances.size());
			memset(&glyphOffsets[0], 0, sizeof(glyphOffsets[0])*glyphOffsets.size());
			memset(&charCluster[0], 0, sizeof(charCluster[0])*charCluster.size());

			for (size_t i = 0; i < glyphCount; i++)
			{
				glyphs[i] = (WORD)i;
			}

			if (sa.fRTL)
			{
				size_t currentGlyphCount = 0;
				for (size_t i = 0; i < length; i++)
				{
					if (i == 0 || charLogattrs[i].fCharStop)
						currentGlyphCount++;
					charCluster[i] = (WORD)(glyphCount - currentGlyphCount);
				}
			}
			else
			{
				size_t currentGlyphCount = 0;
				for (size_t i = 0; i < length; i++)
				{
					if (i == 0 || charLogattrs[i].fCharStop)
						currentGlyphCount++;
					charCluster[i] = (WORD)(currentGlyphCount - 1);
				}
			}

			{
				size_t lastCharIndex = 0;
				size_t lastGlyphIndex = 0;
				for (size_t i = 1; i <= length; i++)
				{
					if (charLogattrs[i].fCharStop)
					{
						size_t glyphLength = i - lastCharIndex;//字符的字形长度
						//下面变量未用到
						const wchar_t* glyphText = sa.fRTL ? runText + length - lastCharIndex - glyphLength : runText + lastCharIndex;
						SIZE size;
						GetTextExtentPoint32(dc, runText, glyphLength, &size);//？ runText
						glyphAdvances[lastGlyphIndex] = size.cx;
						lastCharIndex = i;
						lastGlyphIndex++;
					}
				}
			}

			for (size_t i = 0; i < glyphCount; i++)
			{
				runAbc.abcB += glyphAdvances[i];
			}
		}

		bool UGlyphData::BuildData(HDC dc, SCRIPT_ITEM* scriptItem, SCRIPT_CACHE& scriptCache, const wchar_t* runText, size_t length, std::vector<int>& breakings, std::vector<bool>& breakingAvailabilities)
		{
			bool is_error = false;
			bool realloac_space = false;
			//分配空间
			size_t glyphCount = glyphs.size();
			if (glyphCount == 0)
			{
				size_t glyphCount = length*1.5 + 16;
				glyphs.resize(glyphCount);
				glyphVisattrs.resize(glyphCount);
				charCluster.resize(length);
				realloac_space = true;
			}

			//获取字形信息
			HDC run_dc = NULL;
			while (!is_error)
			{
				int availableGlyphCount = 0;
				HRESULT hr = ScriptShape(
					run_dc,
					&scriptCache,
					runText,
					(int)length,
					(int)glyphCount,
					&sa,
					&glyphs[0],
					&charCluster[0],
					&glyphVisattrs[0],
					&availableGlyphCount
					);

				if (hr == 0)
				{
					glyphCount = availableGlyphCount;
					if (realloac_space)
					{
						glyphs.resize(glyphCount);
						glyphVisattrs.resize(glyphCount);
						glyphAdvances.resize(glyphCount);
						glyphOffsets.resize(glyphCount);
					}
					break;
				}
				else if (hr == E_PENDING)
				{
					run_dc = dc;
				}
				else if (hr == E_OUTOFMEMORY)
				{
					if (realloac_space)
					{
						glyphCount += length;
						glyphs.resize(glyphCount);
						glyphVisattrs.resize(glyphCount);
					}
					else
					{
						is_error = true;
					}
				}
				else if (hr == USP_E_SCRIPT_NOT_IN_FONT)
				{
					if (sa.eScript != SCRIPT_UNDEFINED)
					{
						sa.eScript = SCRIPT_UNDEFINED;
					}
					else
					{
						is_error = true;
					}
				}
				else
				{
					is_error = true;
				}
			}

			if (is_error)
				return false;

			//
			SCRIPT_FONTPROPERTIES fp;
			memset(&fp, 0, sizeof(fp));
			fp.cBytes = sizeof(fp);
			HRESULT hr = ScriptGetFontProperties(
				run_dc,
				&scriptCache,
				&fp);
			WORD invalidGlyph = fp.wgDefault;
			if (hr != S_OK)
			{
				invalidGlyph = 0;
			}

			//生成breaking information
			breakings.push_back(0);
			size_t charIndex = 0;
			bool lastGlyphAvailable = false;
			while (charIndex < length)
			{
				size_t glyphIndex = charCluster[charIndex];
				size_t nextCharIndex = charIndex;
				while (nextCharIndex < length && charCluster[nextCharIndex] == glyphIndex)
				{
					nextCharIndex++;
				}

				int glyphCount = 0;
				if (nextCharIndex == length)
				{
					glyphCount = glyphs.size() - glyphIndex;
				}
				else
				{
					glyphCount = charCluster[nextCharIndex] - glyphIndex;
				}

				if (scriptItem->a.fRTL)
				{
					glyphCount = -glyphCount;
					glyphIndex -= glyphCount - 1;
				}

				bool available = true;
				for (int i = 0; i < glyphCount; i++)
				{
					if (glyphs[i + glyphIndex] == invalidGlyph)
					{
						available = false;
					}
				}

				if (charIndex == 0)
				{
					lastGlyphAvailable = available;
					breakingAvailabilities.push_back(available);
				}
				else if (lastGlyphAvailable != available)
				{
					breakings.push_back(charIndex);
					lastGlyphAvailable = available;
					breakingAvailabilities.push_back(available);
				}

				charIndex = nextCharIndex;
			}

			if (breakings.size() == 1)
			{//之前直接加入了一个
				run_dc = NULL;
				while (!is_error)
				{
					HRESULT hr = ScriptPlace(
						run_dc,
						&scriptCache,
						&glyphs[0],
						(int)glyphCount,
						&glyphVisattrs[0],
						&sa,
						&glyphAdvances[0],
						&glyphOffsets[0],
						&runAbc
						);
					if (hr == 0)
					{
						break;
					}
					else if (hr == E_PENDING)
					{
						run_dc = dc;
					}
					else
					{
						is_error = true;
					}
				}
			}
			return !is_error;
		}

		void UGlyphData::ClearData(size_t glyphCount, size_t length)
		{
			glyphs.resize(glyphCount);
			glyphVisattrs.resize(glyphCount);
			glyphAdvances.resize(glyphCount);
			glyphOffsets.resize(glyphCount);
			charCluster.resize(length);
			memset(&runAbc, 0, sizeof(runAbc));
			memset(&sa, 0, sizeof(sa));
		}

		UGlyphData::UGlyphData()
		{
			ClearData(0, 0);
		}


		void UTextRun::Render(HDC dc, size_t fragmentBoundsIndex, int offsetX, int offsetY, bool renderBackground)
		{
			RunFragmentBounds& fragment = fragmentBounds[fragmentBoundsIndex];
			if (fragment.length == 0)
				return;

			size_t startFromFragmentBounds = 0;
			size_t accumulatedWidth = 0;
			while (startFromFragmentBounds < fragment.length)
			{
				size_t charIndex = fragment.startFromRun + startFromFragmentBounds;
				size_t charLength = 0;
				size_t cluster = 0;
				size_t nextCluster = 0;
				SearchSingleGlyphCluster(charIndex, charLength, cluster, nextCluster);

				size_t clusterStart = 0;
				size_t clusterCount = 0;
				if (scriptItem->IsRightToLeft())
				{
					clusterStart = nextCluster + 1;
					clusterCount = cluster - nextCluster;
				}
				else
				{
					clusterStart = cluster;
					clusterCount = nextCluster - cluster;
				}

				size_t clusterWidth = 0;
				for (size_t i = 0; i < clusterCount; i++)
				{
					clusterWidth += wholeGlyph.glyphAdvances[i + clusterStart];
				}

				int x = 0;
				if (scriptItem->IsRightToLeft())
				{
					x = fragment.bounds.right() - accumulatedWidth - clusterWidth;
				}
				else
				{
					x = fragment.bounds.x() + accumulatedWidth;
				}
				RECT rect;
				rect.left = (int)(x + offsetX);
				rect.top = (int)(fragment.bounds.y() + offsetY);
				rect.right = (int)(rect.left + clusterWidth);
				rect.bottom = (int)(rect.top + fragment.bounds.height()*1.5);

				UColor color = documentFragment->GetCharColor(charIndex + startFromFragment);
				if (renderBackground)
				{
					Color backgroundColor = color.backgroundColor;

					if (ColorGetA(backgroundColor) > 0)
					{
						HBRUSH hbrush = ::CreateSolidBrush(backgroundColor);
						::FillRect(dc, &rect, hbrush);
						::DeleteObject(hbrush);
					}
				}
				else
				{
					Color fontColor = color.fontColor;
					::SelectObject(dc, documentFragment->font.ToHFONT());
					::SetTextColor(dc, fontColor);

					if (needFontFallback)
					{
						::TextOut(dc, rect.left, rect.top, runText + charIndex, charLength);
					}
					else
					{
						ScriptTextOut(
							dc,
							&scriptCache,
							rect.left,
							rect.top,
							0,
							&rect,
							&wholeGlyph.sa,
							NULL,
							0,
							&wholeGlyph.glyphs[clusterStart],
							(int)(clusterCount),
							&wholeGlyph.glyphAdvances[clusterStart],
							NULL,
							&wholeGlyph.glyphOffsets[clusterStart]
							);
					}
				}

				startFromFragmentBounds += charLength;
				accumulatedWidth += clusterWidth;
			}
		}

		void UTextRun::SearchForLineBreak(size_t tempStart, size_t maxWidth, bool firstRun, size_t& charLength, int& charAdvances)
		{
			size_t width = 0;
			charLength = 0;
			charAdvances = 0;
			for (size_t i = tempStart; i <= length;)
			{
				if (i == length || scriptItem->charLogattrs[i + (startFromLine - scriptItem->startFromLine)].fSoftBreak == TRUE)
				{
					if (width <= maxWidth || (firstRun && charLength == 0))
					{
						charLength = i - tempStart;
						charAdvances = width;
					}
					else
					{
						return;
					}
				}
				if (i == length) break;

				int cluster = wholeGlyph.charCluster[i];
				size_t clusterLength = 1;
				while (i + clusterLength < length)
				{
					if (wholeGlyph.charCluster[i + clusterLength] == cluster)
					{
						clusterLength++;
					}
					else
					{
						break;
					}
				}

				if (scriptItem->IsRightToLeft())
				{
					size_t nextCluster
						= i + clusterLength == length
						? -1
						: wholeGlyph.charCluster[i + clusterLength];
					for (size_t j = cluster; j > nextCluster; j--)
					{
						width += wholeGlyph.glyphAdvances[j];
					}
				}
				else
				{
					size_t nextCluster
						= i + clusterLength == length
						? wholeGlyph.glyphs.size()
						: wholeGlyph.charCluster[i + clusterLength];
					for (size_t j = cluster; j < nextCluster; j++)
					{
						width += wholeGlyph.glyphAdvances[j];
					}
				}
				i += clusterLength;
			}
		}

		size_t UTextRun::SumHeight()
		{
			return documentFragment->font.GetHeight();
		}

		size_t UTextRun::SumWidth(size_t charStart, size_t charLength)
		{
			size_t cluster = 0;
			size_t nextCluster = 0;
			SearchGlyphCluster(charStart, charLength, cluster, nextCluster);
			size_t width = 0;
			if (scriptItem->IsRightToLeft())
			{
				for (size_t i = cluster; i > nextCluster; i--)
				{
					width += wholeGlyph.glyphAdvances[i];
				}
			}
			else
			{
				for (size_t i = cluster; i < nextCluster; i++)
				{
					width += wholeGlyph.glyphAdvances[i];
				}
			}
			return width;
		}

		bool UTextRun::BuildData(HDC dc, std::vector<int>& breakings)
		{
			ClearData();

			::SelectObject(dc, documentFragment->font.ToHFONT());
			std::vector<bool> breakingAvailabilities;
			if (wholeGlyph.BuildData(dc, &scriptItem->scriptItem, scriptCache,
				runText, length, breakings, breakingAvailabilities))
			{
				if (breakings.size() == 1 && !breakingAvailabilities[0])
				{
					SCRIPT_LOGATTR* charLogattrs = &scriptItem->charLogattrs[0] + startFromLine - scriptItem->startFromLine;
					wholeGlyph.BuildData(dc, &scriptItem->scriptItem, charLogattrs, runText, length);
					needFontFallback = true;
				}
				advance = wholeGlyph.runAbc.abcA + wholeGlyph.runAbc.abcB + wholeGlyph.runAbc.abcC;

				return true;
			}

			ClearData();
			return false;
		}

		void UTextRun::SearchGlyphCluster(size_t charStart, size_t charLength, size_t& cluster, size_t& nextCluster)
		{
			cluster = wholeGlyph.charCluster[charStart];
			if (charStart + charLength > length)
			{
				charLength = length - charStart;
			}
			if (scriptItem->IsRightToLeft())
			{
				nextCluster
					= charStart + charLength == length
					? -1
					: wholeGlyph.charCluster[charStart + charLength];
			}
			else
			{
				nextCluster
					= charStart + charLength == length
					? wholeGlyph.glyphs.size()
					: wholeGlyph.charCluster[charStart + charLength];
			}
		}

		void UTextRun::SearchSingleChar(size_t charStart, size_t& charLength, size_t& cluster, size_t& nextCluster)
		{
			charLength = 0;
			size_t startFromItem = charStart + startFromLine - scriptItem->startFromLine;
			size_t currentFromItem = startFromItem;
			while (++currentFromItem < scriptItem->length)
			{
				if (scriptItem->charLogattrs[currentFromItem].fCharStop)
					break;
			}
			charLength = currentFromItem - startFromItem;
			SearchGlyphCluster(charStart, charLength, cluster, nextCluster);
		}

		void UTextRun::SearchSingleGlyphCluster(size_t charStart, size_t& charLength, size_t& cluster, size_t& nextCluster)
		{
			cluster = wholeGlyph.charCluster[charStart];
			size_t nextChar = charStart;
			while (nextChar < wholeGlyph.charCluster.size())
			{
				if (wholeGlyph.charCluster[nextChar] != cluster)
					break;
				nextChar++;
			}
			charLength = nextChar - charStart;
			SearchGlyphCluster(charStart, charLength, cluster, nextCluster);
		}

		void UTextRun::ClearData()
		{
			if (scriptCache)
			{
				ScriptFreeCache(&scriptCache);
				scriptCache = NULL;
			}
			advance = 0;
			needFontFallback = false;
			wholeGlyph.ClearData(0, 0);
		}

		UTextRun::~UTextRun()
		{
			ClearData();
		}

	}
}