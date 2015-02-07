#include "stdafx.h"
#include "render_engine.h"
#include "render_target_cairo.h"
#include "render_image_cairo.h"
#include "render_font_cairo.h"

#include "resource/resource_loader.h"

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

	scoped_refptr<RenderFont> RenderEngine::CreateRenderFont(const UIFont& font)
	{
		return RenderFontCairo::Create(font);
	}

}