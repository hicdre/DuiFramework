#pragma once
#include "render/render_target.h"
#include "render/render_image.h"

namespace ui
{
	class Widget;
	class RenderEngine
	{
	public:
		static void InitEngine();
		static void UnintEngine();

		static scoped_refptr<RenderTarget> 
			CreateRenderTarget(Widget* w);

		static scoped_refptr<RenderImage>
			CreateRenderImage(const URL& path);
	};
}