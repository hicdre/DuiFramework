#pragma once
#include "uikit/render/cairo/cairo_forward.h"
#include "uikit/common/ui_font.h"
#include <unordered_map>

namespace ui
{
	class UICairoFontManager
	{
	public:
		UICairoFontManager();
		~UICairoFontManager();

		cairo_font_face_t* GetFont(const UIFont* font);

	private:
		void ClearFonts();
		cairo_font_face_t* FindFont(const UIFont* font);
		cairo_font_face_t* CreateFont(const UIFont* font);
		std::unordered_map<std::wstring, cairo_font_face_t*> fonts_;
	};
}