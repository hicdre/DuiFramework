#include "stdafx.h"
#include "render_engine.h"
#include "render_target_cairo.h"
#include "render_image_cairo.h"

#include "utils/resource_loader.h"

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

	scoped_refptr<RenderImage> RenderEngine::CreateRenderImage(const URL& path)
	{
		std::string contents;
		if (!ResourceLoader::Get()->LoadFileToString(path, &contents))
			return NULL;

		return new RenderImageCairo((void*)contents.c_str(), contents.size());
	}

}