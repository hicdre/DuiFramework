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

		struct ColorRangeItem
		{
			Range range;
			UColor color;
			ColorRangeItem* prev{ NULL };
			ColorRangeItem* next{ NULL };
			ColorRangeItem(size_t length);
			ColorRangeItem(size_t begin, size_t end, UColor c);
		};

		struct ColorRangeList
		{
			ColorRangeItem* first{ NULL };
			ColorRangeItem* last{ NULL };
			~ColorRangeList();

			ColorRangeItem* PushBack(size_t start, size_t end, const UColor& c);
			//p为NULL，表示在first前插入
			ColorRangeItem* InsertAfter(ColorRangeItem* p, size_t start, size_t end, const UColor& c);
			//返回p->prev, 当删除的为第一个时返回NULL
			ColorRangeItem* Remove(ColorRangeItem* p);

			void Clear();

		};

		//字符串格式片段
		struct Fragment
		{
			ColorRangeList colors;
			Font font;
			std::wstring text;

			Fragment(const std::wstring& t);

			void CutColors(size_t start, size_t length);

			void UpdateFontColor(size_t start, size_t length, Color color);

			void UpdateBackgroundColor(size_t start, size_t length, Color color);

			void DefragmentColors();
		
			UColor GetCharColor(size_t index);

			Fragment* Copy(size_t start, size_t length);
		
		};

		struct GlyphData
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

			GlyphData()
			{
				ClearData(0, 0);
			}

			void ClearData(size_t glyphCount, size_t length)
			{
				glyphs.resize(glyphCount);
				glyphVisattrs.resize(glyphCount);
				glyphAdvances.resize(glyphCount);
				glyphOffsets.resize(glyphCount);
				charCluster.resize(length);
				memset(&runAbc, 0, sizeof(runAbc));
				memset(&sa, 0, sizeof(sa));
			}

			bool BuildData(HDC dc, 
				SCRIPT_ITEM* scriptItem, SCRIPT_CACHE& scriptCache, 
				const wchar_t* runText, size_t length, 
				std::vector<int>& breakings, 
				std::vector<bool>& breakingAvailabilities)
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

					size_t glyphCount = 0;
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
					for (size_t i = 0; i < glyphCount; i++)
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
		};
	}
}