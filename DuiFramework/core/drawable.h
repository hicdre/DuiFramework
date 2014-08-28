#pragma once
#include "render/painter.h"
#include "render/rect.h"

namespace ui
{
	class Drawable
	{
	public:
		virtual ~Drawable() {}

		virtual void DoPaint(Painter* painter, const Rect& dest) = 0;
	};
}