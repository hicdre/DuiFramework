#pragma once

#include "base/basictypes.h"
#include "utils/image_file.h"

#include <string>

#include <gdiplus.h>

namespace ui
{
	class Widget;
	class RenderContext
	{
	public:
		RenderContext(Widget* widget);
		~RenderContext();

		void Trans(const Matrix& m);

		void PushClip(const Rect& rect);
		void PopClip();

		HDC GetHDC() { return dc_; }

		void FillRect(const Rect& rect, Color color);
		void DrawArc(const Rect& rect, int from, int angles, Color color, int width);

		void DrawImage(ImagePart* clip, const Rect& dest_rect);
		void DrawImage(ImageFile* image, const Rect& src_rect, const Rect& dest_rect);

		void DrawStringRect(const std::wstring& text, const Font& font, Color color, const Rect& rect);
		void DrawStringRectWithFlags(const std::wstring& text, const Font& font, Color color, const Rect& rect, int flags);
		void DrawStringRectWithFlags(const wchar_t* text, size_t len, const Font& font, Color color, const Rect& rect, int flags);

		static void CalcStringRectWithFlags(const std::wstring& text, const Font& font, const Rect& rect, int flags,
			Rect& out, size_t* len = NULL, int* lines = NULL);
		static void CalcStringSizeWithFlags(const std::wstring& text, const Font& font, const Size& sz, int flags,
			Size& out, size_t* len = NULL, int* lines = NULL);
		static void CalcStringSizeWithFlags(const wchar_t* text, size_t text_len, const Font& font, const Size& sz, int flags,
			Size& out, size_t* len = NULL, int* lines = NULL);
	private:
		Widget* widget_;

		HDC dc_;
		PAINTSTRUCT ps_;

		HBITMAP bitmap_;
		HBITMAP bitmap_prev_;

		Gdiplus::Graphics *gdi_;
		Rect rect_;
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
	};
}