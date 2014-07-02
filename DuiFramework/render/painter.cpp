#include "stdafx.h"
#include "painter.h"

#include "core/widget.h"

namespace ui
{


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
		DrawStringRectWithFlags(text, font, color, rect, TEXT_ALIGN_LEFT);
	}

	void Painter::DrawStringRectWithFlags(const std::wstring& text, const Font& font, Color color, const Rect& rect, int flags)
	{

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