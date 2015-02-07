#include "stdafx.h"
#include "cairo_font_manager.h"

namespace ui
{


	UICairoFontManager::UICairoFontManager()
	{

	}

	UICairoFontManager::~UICairoFontManager()
	{
		ClearFonts();
	}

	cairo_font_face_t* UICairoFontManager::GetFont(const UIFont* font)
	{
		cairo_font_face_t* f = FindFont(font);
		if (f)
			return f;

		f = CreateFont(font);
		
		return f;
	}

	void UICairoFontManager::ClearFonts()
	{
		for (auto iter : fonts_)
		{
			cairo_font_face_destroy(iter.second);
		}
	}

	cairo_font_face_t* UICairoFontManager::FindFont(const UIFont* font)
	{
		auto iter = fonts_.find(font->description());
		if (iter != fonts_.end())
			return iter->second;
		return NULL;
	}

	cairo_font_face_t* UICairoFontManager::CreateFont(const UIFont* font)
	{
		HFONT hf = ::CreateFont(-font->font_size(),
			0, 0, 0,
			font->isBold() ? FW_BOLD : FW_NORMAL, font->isItalic(), font->isUnderline(), FALSE,
			DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_DONTCARE,
			font->font_name().c_str());

		LOGFONTW logfont;
		GetObjectW(hf, sizeof(logfont), &logfont);
		cairo_font_face_t* f = cairo_win32_font_face_create_for_logfontw(&logfont);
		::DeleteObject(hf);
		
		fonts_[font->description()] = f;
		return f;
	}

}