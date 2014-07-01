#pragma once

#include "base/basictypes.h"
#include "render/point.h"
#include "render/size.h"
#include "render/transform.h"
#include "render/rect.h"
#include "render/color.h"

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