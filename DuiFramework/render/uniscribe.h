#pragma once
#include "base/basictypes.h"
#include "render/color.h"
#include "render/rect.h"
#include "render/font.h"
#include "render/range.h"
#include <list>
#include <vector>
#include <Usp10.h>

namespace ui
{
	namespace uniscribe
	{
		struct UColor
		{
			Color fontColor{ 0 };
			Color backgroundColor{ 0 };
			bool operator==(const UColor& other) const;

			bool operator!=(const UColor& other) const;
		};

		struct UColorRangeItem
		{
			Range range;
			UColor color;
			UColorRangeItem* prev{ NULL };
			UColorRangeItem* next{ NULL };
			UColorRangeItem(size_t length);
			UColorRangeItem(size_t begin, size_t end, UColor c);
		};

		struct UColorRangeList
		{
			UColorRangeItem* first{ NULL };
			UColorRangeItem* last{ NULL };
			~UColorRangeList();

			UColorRangeItem* PushBack(size_t start, size_t end, const UColor& c);
			//p为NULL，表示在first前插入
			UColorRangeItem* InsertAfter(UColorRangeItem* p, size_t start, size_t end, const UColor& c);
			//返回p->prev, 当删除的为第一个时返回NULL
			UColorRangeItem* Remove(UColorRangeItem* p);

			void Clear();

		};

		//字符串格式片段
		struct UFragment
		{
			UColorRangeList colors;
			Font font;
			std::wstring text;

			UFragment(const std::wstring& t);

			void CutColors(size_t start, size_t length);

			void UpdateFontColor(size_t start, size_t length, Color color);

			void UpdateBackgroundColor(size_t start, size_t length, Color color);

			void DefragmentColors();
		
			UColor GetCharColor(size_t index);

			UFragment* Copy(size_t start, size_t length);
		
		};

		struct UGlyphData
		{
			//下面4个为字形数组
			std::vector<WORD>				glyphs;
			std::vector<SCRIPT_VISATTR>		glyphVisattrs;
			std::vector<int>				glyphAdvances;
			std::vector<GOFFSET>			glyphOffsets;
			//字符索引
			std::vector<WORD>				charCluster;
			ABC								runAbc;
			SCRIPT_ANALYSIS					sa;

			UGlyphData();

			void ClearData(size_t glyphCount, size_t length);

			bool BuildData(HDC dc, 
				SCRIPT_ITEM* scriptItem, SCRIPT_CACHE& scriptCache, 
				const wchar_t* runText, size_t length, 
				std::vector<int>& breakings, 
				std::vector<bool>& breakingAvailabilities);

			void BuildData(HDC dc, 
				SCRIPT_ITEM* scriptItem, 
				SCRIPT_LOGATTR* charLogattrs, 
				const wchar_t* runText, size_t length);
		};

		struct UItem
		{
			SCRIPT_ITEM						scriptItem;
			size_t							startFromLine{ 0 };
			size_t							length{ 0 };
			const wchar_t*					itemText{ 0 };
			std::vector<SCRIPT_LOGATTR>			charLogattrs;

			void ClearData()
			{
				charLogattrs.clear();
			}

			bool BuildData()
			{
				// generate break information
				charLogattrs.resize(length);

				HRESULT hr = ScriptBreak(
					itemText,
					(int)length,
					&scriptItem.a,
					&charLogattrs[0]
					);
				if (hr == 0)
					return true;
				ClearData();
				return false;
			}

			bool IsRightToLeft()
			{
				return scriptItem.a.fRTL;
			}
		};

		struct UTextRun
		{
			struct RunFragmentBounds
			{
				size_t						startFromRun;
				size_t						length;
				Rect						bounds;
			};

			//***************************** Document Data
			UFragment*				documentFragment{ 0 };
			UItem*					scriptItem{ 0 };
			//***************************** Uniscribe Data
			size_t							startFromLine{ 0 };
			size_t							startFromFragment{ 0 };
			size_t							length{ 0 };
			const wchar_t*					runText{ 0 };

			SCRIPT_CACHE					scriptCache{ 0 };
			size_t							advance{ 0 };
			UGlyphData				wholeGlyph;
			bool							needFontFallback{ 0 };
			//***************************** Layout Data
			std::vector<RunFragmentBounds>			fragmentBounds;

			~UTextRun();

			void ClearData();

			void SearchSingleGlyphCluster(size_t charStart, size_t& charLength, 
				size_t& cluster, size_t& nextCluster);

			void SearchSingleChar(size_t charStart, size_t& charLength, 
				size_t& cluster, size_t& nextCluster);

			void SearchGlyphCluster(size_t charStart, size_t charLength, 
				size_t& cluster, size_t& nextCluster);

			bool BuildData(HDC dc, std::vector<int>& breakings);

			size_t SumWidth(size_t charStart, size_t charLength);

			size_t SumHeight();

			void SearchForLineBreak(size_t tempStart, size_t maxWidth,
				bool firstRun, size_t& charLength, int& charAdvances);

			void Render(HDC dc, 
				size_t fragmentBoundsIndex, 
				int offsetX, int offsetY, bool renderBackground);
		};

	}
}