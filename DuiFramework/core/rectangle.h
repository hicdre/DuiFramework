#pragma once
#include "render/rect.h"
#include "render/image.h"
#include "render/color.h"
#include "core/view.h"

#include <string>

namespace ui
{
	class Painter;
	class SolidRectangle : public View
	{
	public:
		SolidRectangle();
		SolidRectangle(Color color);

		void SetColor(Color color);
		void SetImageId(const std::string& id);

		virtual void OnPaint(Painter* painter) override;
	private:
		Color color_{ 0xFFFFFFFF };
		std::string id_;
	};


}