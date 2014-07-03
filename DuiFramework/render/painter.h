#pragma once

#include "base/basictypes.h"
#include "render/point.h"
#include "render/size.h"
#include "render/transform.h"
#include "render/rect.h"
#include "render/color.h"
#include "render/image.h"
#include "render/font.h"

#include <string>

namespace ui
{
	class Widget;
	class Painter
	{
	public:
		Painter(Widget* widget);
		~Painter();

		void Trans(const Transform& m);

		void FillRect(const Rect& rect, Color color);
		void DrawLine(const Rect& rect, int line_size, DWORD color, int nStyle = PS_SOLID);

		void DrawImage(ImageClip* clip, const Rect& dest_rect);
		void DrawImage(Image* image, const Rect& src_rect, const Rect& dest_rect);

		void DrawStringRect(const std::wstring& text, const Font& font, Color color, const Rect& rect);
		void DrawStringRectWithFlags(const std::wstring& text, const Font& font, Color color, const Rect& rect, int flags);

		static void CalcStringRectWithFlags(const std::wstring& text, const Font& font, const Rect& rect, int flags,
			Rect& out, size_t* len = NULL, int* lines = NULL);
	private:
		Widget* widget_;

		HDC dc_;
		PAINTSTRUCT ps_;
	};

	class ScopedPainter
	{
	public:
		ScopedPainter(Painter* painter, const Transform& m);
		~ScopedPainter();
	private:
		Transform m_;
		Painter* p_;
	};
}