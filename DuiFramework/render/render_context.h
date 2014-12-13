#pragma once

#include "base/basictypes.h"
#include "utils/image_file.h"
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

		void DrawImage(ImagePart* clip, const Rect& dest_rect);
		void DrawImage(ImageFile* image, const Rect& src_rect, const Rect& dest_rect);
	private:
		scoped_refptr<RenderTarget> target_;
	};

	
}