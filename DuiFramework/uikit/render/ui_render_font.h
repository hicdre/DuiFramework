#pragma once
#include "uikit/common/ui_font.h"
#include "uikit/common/ui_glyphs.h"

namespace ui
{
	class UIRenderFont : public RefCounted < UIRenderFont >
	{
	public:
		virtual ~UIRenderFont() {}

		virtual void GetGlyphs(const wchar_t* text, size_t len, 
			UIGlyph** glyphs, size_t& glyphsCount) = 0;

		virtual int MeasureWidth(const wchar_t* text, size_t len) = 0;

		virtual double GetHeight() = 0;
		virtual double GetAscent() = 0;
		virtual double GetDescent() = 0;
	};
}