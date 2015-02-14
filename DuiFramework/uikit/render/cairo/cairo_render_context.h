#pragma once
//#include "render/render_engine.h"
#include "uikit/render/ui_render_context.h"
#include "uikit/render/cairo/cairo_forward.h"



namespace ui
{
	class UICairoRenderEngine;
	class UICairoRenderContext : public UIRenderContext
	{
	public:
		UICairoRenderContext(HWND hwnd, UICairoRenderEngine* engine);
		~UICairoRenderContext();

		//virtual scoped_refptr<UIRenderPath> OpenRenderPath() override;

		virtual void ClipRect(const Rect& rect)  override;
		virtual void ClipPath(const UIRenderPath* path)  override;

		virtual void StorkeRect(const Rect& rect, Color color, uint32 size) override;
		virtual void StorkePath(const UIRenderPath* path, Color color, uint32 size) override;

		virtual void FillRect(const Rect& rect, Color color) override;
		virtual void FillPath(const UIRenderPath* path, Color color) override;

		//virtual void DrawImage(const RenderImage *image, const Rect& dest) override;
		//virtual void DrawImage(const RenderImage *image, const Rect& src, const Rect& dest) override;
		virtual void ShowGlyphs(const UIGlyph* glyph, size_t len, UIRenderFont* font, Color color) override;
		//virtual void ShowGlyphs(const UIGlyphBuffer& buffer, const UIFont* font, Color color) override;
// 		virtual void ShowGlyphs(const GlyphBuffer& buffer, const UIFont& font, Color color) override;
// 		virtual void DrawText(const std::wstring& buffer, const UIFont& font, Color color) override;
		virtual void DrawText(const std::wstring& text, const UIFont* font, Color color) override;

		virtual void Trans(const Matrix& m) override;
		virtual void Translate(double dx, double dy) override;
		virtual void PushState() override;
		virtual void PopState() override;

		void NewPath();
		void PeparePath(const UIRenderPath* path);

		void InitColor(Color color);
		cairo_t* get_context() const { return cairo_; }
	private:
		void BeginPaint();
		void EndPaint();

		

		UICairoRenderEngine* engine() const { return engine_; }

		UICairoRenderEngine* engine_;

		HWND hwnd_;
		PAINTSTRUCT ps_;

		cairo_t* cairo_;
		cairo_surface_t* surface_;
		Rect rect_;
	};
}