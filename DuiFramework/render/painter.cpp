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