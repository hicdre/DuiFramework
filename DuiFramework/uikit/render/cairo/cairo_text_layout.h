#pragma once

#include "uikit/render/ui_text_layout.h"
#include "uikit/render/cairo/cairo_forward.h"
#include "uikit/render/ui_render_font.h"
#include "uikit/text/text_storage.h"

namespace ui
{
	class UIRenderContext;
	class UICairoRenderEngine;
	class UICairoTextPagraph;
	

	class UICairoTextLayout : public UITextLayout
	{
	public:
		UICairoTextLayout(const UIFont* font, const Rect& rect, UICairoRenderEngine* engine);
		virtual ~UICairoTextLayout() override;

		//associate text
		virtual void SetText(const std::wstring& str) override;
		virtual void SetFont(const UIFont* font) override;
		virtual void SetLineBreakMode(UILineBreakMode mode) override;
		virtual void SetBounds(const Rect& rect) override;
		virtual Rect Layout() override;
		virtual void Render(UIRenderContext* context, Color color) override;

	private:
		void RealLayout();
		void CleanLayoutResult();
		UICairoRenderEngine* engine() const { return engine_; }
		UICairoRenderEngine* engine_;
		scoped_refptr<UIFont> font_;
		UILineBreakMode lineBreakMode_;
		Rect layoutBounds_;
		Rect textBounds_;
		std::wstring contents_;

		//result
		UITextStorage* textStorage_;

		bool needLayout_;
	};

}
