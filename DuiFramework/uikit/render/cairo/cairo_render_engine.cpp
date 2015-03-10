#include "stdafx.h"
#include "cairo_render_engine.h"
#include "cairo_render_context.h"
#include "cairo_text_layout.h"
#include "cairo_render_font.h"

namespace ui
{

	UICairoRenderEngine::UICairoRenderEngine()
	{
		cairo_startup();
	}

	UICairoRenderEngine::~UICairoRenderEngine()
	{
		font_manager_.reset();
		cairo_shutdown();
	}

	scoped_refptr<UIRenderContext> UICairoRenderEngine::CreateRenderContextForHWND(HWND hwnd)
	{
		return new UICairoRenderContext(hwnd, this);
	}

	scoped_refptr<UITextLayout> UICairoRenderEngine::CreateRenderTextLayout(const UIFont* font,
		const Rect& bounds)
	{
		scoped_refptr<UICairoTextLayout> layout(new UICairoTextLayout(font, bounds, this));
		return layout.get();
	}

	scoped_refptr<UITextLayout> UICairoRenderEngine::CreateRenderTextLayout(const UIFont* font,
		const Rect& bounds,
		const std::wstring& str)
	{
		scoped_refptr<UICairoTextLayout> layout(new UICairoTextLayout(font, bounds, this));
		return layout.get();
	}

	UICairoFontManager* UICairoRenderEngine::font_manager()
	{
		if (!font_manager_.get())
		{
			font_manager_.reset(new UICairoFontManager);
		}
		return font_manager_.get();
	}

	scoped_refptr<UIRenderFont> UICairoRenderEngine::CreateRenderFont(UIFont* font)
	{
		return new UICairoRenderFont(font, this);
	}

}