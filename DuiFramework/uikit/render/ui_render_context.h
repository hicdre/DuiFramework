#pragma once
#include "uikit/render/ui_render_path.h"
#include "uikit/render/ui_render_font.h"
//#include "uikit/render/ui_render_glyphs.h"
//#include "render/render_image.h"

namespace ui
{
	class UIRenderContext : public RefCounted<UIRenderContext>
	{
	public:
		virtual ~UIRenderContext() {}

		//virtual scoped_refptr<UIRenderPath> OpenRenderPath() = 0;

		virtual void ClipRect(const Rect& rect) = 0;

		virtual void ClipPath(const UIRenderPath* path) = 0;

		virtual void StorkeRect(const Rect& rect, Color color, uint32 size) = 0;
		virtual void StorkePath(const UIRenderPath* path, Color color, uint32 size) = 0;

		virtual void FillRect(const Rect& rect, Color color) = 0;
		virtual void FillPath(const UIRenderPath* path, Color color) = 0;

// 		virtual void DrawImage(const RenderImage *image, const Rect& dest) = 0;
// 		virtual void DrawImage(const RenderImage *image, const Rect& src, const Rect& dest) = 0;

		virtual void ShowGlyphs(const UIGlyph* glyph, size_t len, UIRenderFont* font, Color color) = 0;
//		virtual void ShowGlyphs(const UIGlyphBuffer& buffer, const UIFont* font, Color color) = 0;

		virtual void DrawText(const std::wstring& text, const UIFont* font, Color color) = 0;

		virtual void Trans(const Matrix& m) = 0;
		virtual void Translate(double dx, double dy) = 0;

		virtual void PushState() = 0;
		virtual void PopState() = 0;

	};

	class UIScopedRender
	{
	public:
		UIScopedRender(UIRenderContext* p);
		~UIScopedRender();
	private:
		UIRenderContext* p_;
	};

	

	class UIScopedPainter
	{
	public:
		UIScopedPainter(UIRenderContext* painter, const Matrix& m);
		~UIScopedPainter();
	private:
		Matrix m_;
		UIRenderContext* p_;
	};

	class UIScopedClipper
	{
	public:
		UIScopedClipper(UIRenderContext* painter, const Rect& r);
		UIScopedClipper(UIRenderContext* painter, const UIRenderPath* r);
		~UIScopedClipper();
	private:
		UIRenderContext* p_;
	};
}
