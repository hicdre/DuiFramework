#include "stdafx.h"
#include "render_engine.h"
#include "render_target_cairo.h"

namespace ui
{


	void RenderEngine::InitEngine()
	{
		CairoEngine::Init();
	}

	void RenderEngine::UnintEngine()
	{
		CairoEngine::Unint();
	}

	scoped_refptr<RenderTarget> RenderEngine::CreateRenderTarget(Widget* w)
	{
		return new RenderTargetCairo(w);
	}

}