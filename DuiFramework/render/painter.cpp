#include "stdafx.h"
#include "painter.h"

#include "base/scoped_ptr.h"
#include "core/widget.h"
#include "core/constants.h"
//#include "render/render_text.h"

#include <vector>

namespace ui
{
	namespace
	{
		void GetStringFormat(Gdiplus::StringFormat& format, int flags)
		{
			//水平布局
			if (flags & TEXT_CENTER) 
				format.SetAlignment(Gdiplus::StringAlignmentCenter);
			else if (flags & TEXT_RIGHT)
				format.SetAlignment(Gdiplus::StringAlignmentFar);
			else 
				format.SetAlignment(Gdiplus::StringAlignmentNear);
			//垂直布局
			if (flags & TEXT_VCENTER)
				format.SetLineAlignment(Gdiplus::StringAlignmentCenter);
			else if (flags & TEXT_BOTTOM)
				format.SetLineAlignment(Gdiplus::StringAlignmentFar);
			else
				format.SetLineAlignment(Gdiplus::StringAlignmentNear);

			if (flags & TEXT_NOCLIP)
				format.SetFormatFlags(Gdiplus::StringFormatFlagsNoClip);

			if (flags & TEXT_PATH_ELLIPSIS)
				format.SetTrimming(Gdiplus::StringTrimmingEllipsisPath);
			else if (flags & TEXT_END_ELLIPSIS)
				format.SetTrimming(Gdiplus::StringTrimmingEllipsisCharacter);
			else if (flags & TEXT_END_WORD_ELLIPSIS)
				format.SetTrimming(Gdiplus::StringTrimmingEllipsisWord);
			else
				format.SetTrimming(Gdiplus::StringTrimmingNone);
		}
	}

	Painter::Painter(Widget* widget)
		: widget_(widget)
	{
		dc_ = BeginPaint(widget->hwnd(), &ps_);
		SetGraphicsMode(dc_, GM_ADVANCED);
		SetMapMode(dc_, MM_TEXT);
		SetBkMode(dc_, TRANSPARENT);
	}

	Painter::~Painter()
	{
		EndPaint(widget_->hwnd(), &ps_);
	}

	void Painter::FillRect(const Rect& rect, Color color)
	{
		::SetBkColor(dc_, RGB(GetBValue(color), GetGValue(color), GetRValue(color)));
		::ExtTextOut(dc_, 0, 0, ETO_OPAQUE, &rect.ToRECT(), NULL, 0, NULL);
	}

	void Painter::Trans(const Transform& m)
	{
		::ModifyWorldTransform(dc_, &m.ToXFORM(), MWT_LEFTMULTIPLY);
	}

	void Painter::DrawLine(const Rect& rect, int line_size, DWORD color, int nStyle /*= PS_SOLID*/)
	{
		LOGPEN lg;
		lg.lopnColor = color;
		lg.lopnStyle = nStyle;
		lg.lopnWidth.x = line_size;
		HPEN hPen = CreatePenIndirect(&lg);
		HPEN hOldPen = (HPEN)::SelectObject(dc_, hPen);
		POINT ptTemp = { 0 };
		::MoveToEx(dc_, rect.x(), rect.y(), &ptTemp);
		::LineTo(dc_, rect.right(), rect.bottom());
		::SelectObject(dc_, hOldPen);
		::DeleteObject(hPen);
	}

	void Painter::DrawImage(ImageClip* clip, const Rect& dest_rect)
	{
		Image* image = clip->image();
		if (!image)
			return;
		DrawImage(image, clip->rect(), dest_rect);
	}

	void Painter::DrawImage(Image* image, const Rect& src_rect, const Rect& dest_rect)
	{
		HDC src_dc = ::CreateCompatibleDC(dc_);
		HBITMAP hOldBitmap = (HBITMAP) ::SelectObject(src_dc, image->ToHBITMAP());
		BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
		::AlphaBlend(dc_, dest_rect.x(), dest_rect.y(), dest_rect.width(), dest_rect.height(),
			src_dc, src_rect.x(), src_rect.y(), src_rect.width(), src_rect.height(),
			bf);
		::SelectObject(src_dc, hOldBitmap);
		::DeleteDC(src_dc);
	}

	void Painter::DrawStringRect(const std::wstring& text, const Font& font, Color color, const Rect& rect)
	{
		DrawStringRectWithFlags(text, font, color, rect, TEXT_LEFT | TEXT_VCENTER | TEXT_END_ELLIPSIS);
	}

	void Painter::DrawStringRectWithFlags(const std::wstring& text, const Font& font, Color color, const Rect& rect, int flags)
	{
		Gdiplus::Graphics graphics(dc_);
		Gdiplus::SolidBrush  brush(Gdiplus::Color((unsigned int)color));
		Gdiplus::Font        gdi_font(dc_, font.ToHFONT());
		Gdiplus::StringFormat format;
		Gdiplus::RectF       rectF(rect.x(), rect.y(), rect.width(), rect.height());

		GetStringFormat(format, flags);
		graphics.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
		graphics.DrawString(text.c_str(), text.size(), &gdi_font, rectF, &format, &brush);

	}

	void Painter::CalcStringRectWithFlags(const std::wstring& text, const Font& font, const Rect& rect, int flags, Rect& out, size_t* len, int* lines)
	{
		HDC hdc = GetDC(NULL);
		Gdiplus::Graphics graphics(hdc);
		Gdiplus::Font        gdi_font(hdc, font.ToHFONT());
		Gdiplus::StringFormat format;
		Gdiplus::RectF       rectF(rect.x(), rect.y(), rect.width(), rect.height());

		Gdiplus::RectF outF;

		GetStringFormat(format, flags);
		graphics.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
		graphics.MeasureString(text.c_str(), text.size(), &gdi_font, rectF, &format, &outF, (int*)len, lines);

		out.SetRect(outF.X, outF.Y, outF.Width, outF.Height);
		::ReleaseDC(NULL, hdc);
	}



	ScopedPainter::ScopedPainter(Painter* painter, const Transform& m)
		: p_(painter), m_(m.Invert())
	{
		p_->Trans(m);
	}

	ScopedPainter::~ScopedPainter()
	{
		p_->Trans(m_);
	}

}