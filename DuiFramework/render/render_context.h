#pragma once

#include "base/basictypes.h"
#include "utils/image_file.h"

#include <string>

#include <gdiplus.h>

namespace ui
{
	class Widget;
	class RenderContextPrivate;
	class RenderPath;
	class RenderContext
	{
	public:
		static void Init();
		static void Uninit();
		RenderContext(Widget* widget);
		~RenderContext();

		void Trans(const Matrix& m);

 		void PushState();
 		void PopState();

		RenderPath* CreatePath();

// 
 		void SetClip(const Rect& rect);
// 		void SetClipRgn(HRGN r);
		void FillRect(const Rect& rect, Color color);
		void DrawArc(const Rect& rect, int from, int angles, Color color, int width);

		void DrawImage(ImagePart* clip, const Rect& dest_rect);
		void DrawImage(ImageFile* image, const Rect& src_rect, const Rect& dest_rect);
	private:
		friend class RenderPath;
		RenderContextPrivate* context_;
	};

	class ScopedPainter
	{
	public:
		ScopedPainter(RenderContext* painter, const Matrix& m);
		~ScopedPainter();
	private:
		Matrix m_;
		RenderContext* p_;
	};

	class ScopedClipper
	{
	public:
		ScopedClipper(RenderContext* painter, const Rect& r);
		~ScopedClipper();
	private:
		RenderContext* p_;
		uint32 state_;
	};
}