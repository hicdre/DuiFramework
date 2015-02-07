#pragma once

#include "uikit/render/ui_render_engine.h"
#include "uikit/render/cairo/cairo_forward.h"
#include "uikit/render/cairo/cairo_font_manager.h"

namespace ui
{
	class UICairoRenderEngine : public UIRenderEngine
	{
	public:
		UICairoRenderEngine();
		virtual ~UICairoRenderEngine() override;

		virtual scoped_refptr<UIRenderContext>
			CreateRenderContextForHWND(HWND hwnd) override;

		virtual scoped_refptr<UITextLayout>
			CreateRenderTextLayout(const UIFont* font,
			const Rect& bounds) override;

		virtual scoped_refptr<UITextLayout>
			CreateRenderTextLayout(const UIFont* font,
			const Rect& bounds, 
			const std::wstring& str) override;

		UICairoFontManager* font_manager();
	private:
		scoped_ptr<UICairoFontManager> font_manager_;
	};
}
