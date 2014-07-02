#pragma once
#include "render/rect.h"
#include "render/image.h"
#include "render/color.h"

#include <string>

namespace ui
{
	class View;
	class Painter;
	class Background
	{
	public:
		virtual ~Background() {}
		virtual void DoPaint(View* view, Painter* painter) = 0;
	};

	class NormalBackground : public Background
	{
	public:
		NormalBackground();
		NormalBackground(Color color);
		NormalBackground(const std::string& id);

		void SetColor(Color color);
		void SetImageId(const std::string& id);

		virtual void DoPaint(View* view, Painter* painter) override;
	private:
		Color color_{ 0xFFFFFFFF };
		std::string id_;
	};

}