﻿#include "stdafx.h"
#include "render_font_cairo.h"

namespace ui
{


	RenderFontCairo::RenderFontCairo(const UIFont& font)
		: font_(font)
	{
		Initialize();
	}

	RenderFontCairo::~RenderFontCairo()
	{

	}

	void RenderFontCairo::Initialize()
	{

	}

	scoped_refptr<RenderFont> RenderFontCairo::Create(const UIFont& font)
	{
		return new RenderFontCairo(font);
	}

	void RenderFontCairo::GetGlyphIndices(const RenderContext* context, const wchar_t* str, size_t len, uint32* glyphs)
	{
	}

}