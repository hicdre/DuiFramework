#pragma once
#include "render/render_path.h"
#include "render/render_image.h"

namespace ui
{
	class RenderTarget : public RefCounted < RenderTarget >
	{
	public:
		virtual ~RenderTarget() {}

		virtual scoped_refptr<RenderPath> OpenRenderPath() = 0;

		virtual void ClipRect(const Rect& rect) = 0;

		virtual void ClipPath(const RenderPath* path) = 0;

		virtual void StorkeRect(const Rect& rect, Color color, uint32 size) = 0;

		virtual void StorkePath(const RenderPath* path, Color color, uint32 size) = 0;

		virtual void FillRect(const Rect& rect, Color color) = 0;

		virtual void FillPath(const RenderPath* path, Color color) = 0;

		virtual void DrawImage(const RenderImage *image, const Rect& dest) = 0;

		virtual void Trans(const Matrix& m) = 0;

		virtual void PushState() = 0;

		virtual void PopState() = 0;
	};
}
