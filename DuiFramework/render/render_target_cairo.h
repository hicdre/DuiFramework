#pragma once
#include "render/render_engine.h"
#include "render/render_cairo_forward.h"



namespace ui
{

	class RenderTargetCairo : public RenderTarget
	{
	public:
		RenderTargetCairo(Widget* widget);
		~RenderTargetCairo();

		virtual scoped_refptr<RenderPath> OpenRenderPath() override;

		virtual void ClipRect(const Rect& rect)  override;
		virtual void ClipPath(const RenderPath* path)  override;

		virtual void StorkeRect(const Rect& rect, Color color, uint32 size) override;
		virtual void StorkePath(const RenderPath* path, Color color, uint32 size) override;

		virtual void FillRect(const Rect& rect, Color color) override;
		virtual void FillPath(const RenderPath* path, Color color) override;

		virtual void DrawImage(const RenderImage *image, const Rect& dest) override;
		virtual void DrawImage(const RenderImage *image, const Rect& src, const Rect& dest) override;

		virtual void ShowGlyphs(const GlyphBuffer& buffer, const UIFont& font, Color color) override;
		virtual void DrawText(const std::wstring& buffer, const UIFont& font, Color color) override;

		virtual void Trans(const Matrix& m) override;
		virtual void PushState() override;
		virtual void PopState() override;

		cairo_t* get_context() const { return cairo_; }

		void NewPath();
		void PeparePath(const RenderPath* path);
	private:
		void set_color(Color color);
		Widget* widget_;

		PAINTSTRUCT ps_;

		cairo_t* cairo_;
		cairo_surface_t* surface_;
		Rect rect_;
	};
}