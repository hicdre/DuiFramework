#pragma once
#include "base/basictypes.h"
#define CAIRO_WIN32_STATIC_BUILD 1
#include "third_party/cairo/src/cairo.h"
#include "third_party/cairo/src/cairo-win32.h"
#include "core/widget.h"

namespace ui
{
	void InitCario();
	void UnintCario();

	class RenderContextPrivate
	{
	public:
		RenderContextPrivate(Widget* widget);
		~RenderContextPrivate();

		void Trans(const Matrix& m);

		void PushState();
		void PopState();

		void SetClip(const Rect& rect);
		void SetClipRgn(HRGN r);

		//HDC GetHDC() { return dc_; }

		void FillRect(const Rect& rect, Color color);
		void DrawArc(const Rect& rect, int from, int angles, Color color, int width);


		void NewPath();
		void ClosePath();
	private:
		void set_color(Color color);
		Widget* widget_;

		PAINTSTRUCT ps_;

		cairo_t* cairo_;
		cairo_surface_t* surface_;
		Rect rect_;
	};
}