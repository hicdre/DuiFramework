#pragma once
#include "uikit/common/ui_font.h"
#include "uikit/common/ui_constants.h"

namespace ui
{
	

	class UIRenderContext;
	class UITextLayout : public RefCounted < UITextLayout >
	{
	public:
		virtual ~UITextLayout() {}

		virtual void SetText(const std::wstring& str) = 0;

		virtual void SetLineBreakMode(UILineBreakMode mode) = 0;

		virtual void SetFont(const UIFont* font) = 0;

		virtual void SetBounds(const Rect& rect) = 0;

		virtual Rect Layout() = 0;

		virtual void Render(UIRenderContext* context, Color color) = 0;
	};


}