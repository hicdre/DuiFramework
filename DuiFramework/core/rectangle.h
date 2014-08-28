#pragma once
#include "render/rect.h"
#include "render/color.h"
#include "core/drawable.h"

#include <string>

namespace ui
{
	class Painter;
	class SolidRectangle : public Drawable
	{
	public:
		SolidRectangle();
		SolidRectangle(Color color);

		void SetColor(Color color);
		void SetImageId(const std::string& id);

		virtual void DoPaint(Painter* painter, const Rect& dest) override;
	private:
		Color color_{ 0xFFFFFFFF };
	};


}