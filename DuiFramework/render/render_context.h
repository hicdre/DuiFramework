#pragma once
#include "render/render_engine.h"
#include <string>


namespace ui
{
	class Widget;
	class RenderPath;
	class RenderContext
	{
	public:
		RenderContext(Widget* widget);
		~RenderContext();

		void Trans(const Matrix& m);

 		void PushState();
 		void PopState();

		scoped_refptr<RenderPath> CreatePath();

		void FillPath(const RenderPath* path, Color color);

 		void ClipRect(const Rect& rect);
		void ClipPath(const RenderPath* path);

		void FillRect(const Rect& rect, Color color);
		void DrawArc(const Rect& rect, int from, int angles, Color color, int width);

		void DrawImage(const RenderImage *image, const Rect& dest);
		void DrawImage(const RenderImage *image, const Rect& src, const Rect& dest);

		void DrawText(const std::wstring& text, const Font& font, Color color);
	private:
		scoped_refptr<RenderTarget> target_;
	};

	
}