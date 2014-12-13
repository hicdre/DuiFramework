#pragma once
#include "render/render_target.h"

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
	};
}