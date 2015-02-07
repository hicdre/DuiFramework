#include "stdafx.h"
#include "ui_render_engine.h"
#include "uikit/render/cairo/cairo_render_engine.h"
#include "uikit/ui_application.h"
// #include "render_image_cairo.h"
// #include "render_font_cairo.h"

/*#include "resource/resource_loader.h"*/

namespace ui
{

	UIRenderEngine* UIRenderEngine::Init(Type type)
	{
		if (type == EngineCairo)
		{
			return new UICairoRenderEngine;
		}
		return NULL;
	}

	UIRenderEngine* UIRenderEngine::current()
	{
		return UIApplication::Get()->DefaultRenderEngine();
	}

// 	scoped_refptr<RenderImage> RenderEngine::CreateRenderImage(const URL& path)
// 	{
// 		std::string contents;
// 		if (!ResourceLoader::Get()->LoadFileToString(path, &contents))
// 			return NULL;
// 
// 		return new RenderImageCairo((void*)contents.c_str(), contents.size());
// 	}
// 
// 	scoped_refptr<RenderFont> RenderEngine::CreateRenderFont(const UIFont& font)
// 	{
// 		return RenderFontCairo::Create(font);
// 	}


	UIScopedPainter::UIScopedPainter(UIRenderContext* painter, const Matrix& m)
		: p_(painter), m_(m.Invert())
	{
		p_->PushState();
		p_->Trans(m);
	}

	UIScopedPainter::~UIScopedPainter()
	{
		p_->PopState();
	}


	UIScopedClipper::UIScopedClipper(UIRenderContext* painter, const Rect& r)
		: p_(painter)
	{
		p_->PushState();
		p_->ClipRect(r);
	}

	UIScopedClipper::UIScopedClipper(UIRenderContext* painter, const UIRenderPath* r)
		: p_(painter)
	{
		p_->PushState();
		p_->ClipPath(r);
	}


	UIScopedClipper::~UIScopedClipper()
	{
		p_->PopState();
	}
}