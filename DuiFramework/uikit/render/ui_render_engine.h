#pragma once
#include "uikit/render/ui_render_context.h"
#include "uikit/render/ui_text_layout.h"
#include <windows.h>

namespace ui
{
	class UIRenderEngine
	{
	public:
		enum Type {
			EngineCairo = 0,
			EngineD2D,
		};

		static UIRenderEngine* Init(Type type);

		static UIRenderEngine* current();

		virtual ~UIRenderEngine() {}
		virtual scoped_refptr<UIRenderContext> 
			CreateRenderContextForHWND(HWND hwnd) = 0;


		virtual scoped_refptr<UITextLayout>
			CreateRenderTextLayout(const UIFont* font,
				const Rect& bounds) = 0;

		virtual scoped_refptr<UITextLayout>
			CreateRenderTextLayout(const UIFont* font,
			const Rect& bounds,
			const std::wstring& str) = 0;
	};


}