#include "stdafx.h"
#include "render_context.h"

#include "base/scoped_ptr.h"
#include "core/widget.h"
#include "core/constants.h"
#include "utils/utils.h"
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

			if (flags & TEXT_PATH_ELLIPSIS)
				format.SetTrimming(Gdiplus::StringTrimmingEllipsisPath);
			else if (flags & TEXT_END_ELLIPSIS)
				format.SetTrimming(Gdiplus::StringTrimmingEllipsisCharacter);
			else if (flags & TEXT_END_WORD_ELLIPSIS)
				format.SetTrimming(Gdiplus::StringTrimmingEllipsisWord);
			else
				format.SetTrimming(Gdiplus::StringTrimmingNone);

			int format_flags = 0;
			if (flags & TEXT_NOCLIP)
				format_flags |= Gdiplus::StringFormatFlagsNoClip;
			if (!(flags & TEXT_MUTLILINE))
				format_flags |= Gdiplus::StringFormatFlagsNoWrap;
			format.SetFormatFlags(format_flags);
		}

		

	}

	RenderContext::RenderContext(Widget* widget)
		: widget_(widget)
		, gdi_(NULL)
	{
		HDC wnd_dc = BeginPaint(widget->hwnd(), &ps_);
		SetGraphicsMode(wnd_dc, GM_ADVANCED);
		SetMapMode(wnd_dc, MM_TEXT);
		SetBkMode(wnd_dc, OPAQUE);
		//SetBkMode(wnd_dc, TRANSPARENT);

		dc_ = ::CreateCompatibleDC(wnd_dc);
		//dc_ = wnd_dc;
		SetGraphicsMode(dc_, GM_ADVANCED);
		SetMapMode(dc_, MM_TEXT);
		SetBkMode(dc_, OPAQUE);
		//SetMapMode(dc_, MM_LOENGLISH);
		//SetWorldTransform(dc_, &Matrix().ToXFORM());

		RECT rc;
		::GetClientRect(widget->hwnd(), &rc);
		rect_ = rc;

		bitmap_ = CreateDIB(rect_.width(), rect_.height());
		bitmap_prev_ = (HBITMAP)::SelectObject(dc_, bitmap_);
		gdi_ = new Gdiplus::Graphics(dc_);
	}

	RenderContext::~RenderContext()
	{
 		BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
 		::AlphaBlend(ps_.hdc, 0, 0, rect_.width(), rect_.height(),
 			dc_, 0, 0, rect_.width(), rect_.height(),
 			bf);

//  		::StretchBlt(ps_.hdc, 0, 0, rect_.width(), rect_.height(),
//   			dc_, 0, 0, rect_.width(), rect_.height(),
//   			SRCCOPY);
 		::SelectObject(dc_, bitmap_prev_);
 		::DeleteObject(bitmap_);
 		::DeleteDC(dc_);

		EndPaint(widget_->hwnd(), &ps_);

		if (gdi_)
		{
			delete gdi_;
			gdi_ = NULL;
		}
	}

	void RenderContext::FillRect(const Rect& rect, Color color)
	{
		if (color == Color_Transparent)
			return;
		Gdiplus::SolidBrush  brush(Gdiplus::Color((unsigned int)color));
		gdi_->FillRectangle(&brush, rect.x(), rect.y(), rect.width(), rect.height());
	}


	void RenderContext::DrawArc(const Rect& rect, int from, int angles, Color color, int width)
	{
		if (color == Color_Transparent)
			return;
		Gdiplus::SolidBrush  brush(Gdiplus::Color((unsigned int)color));
		Gdiplus::Pen pen(&brush, width);
		gdi_->DrawArc(&pen, rect.x(), rect.y(), rect.width(), rect.height(), from, angles);

	}


	void RenderContext::Trans(const Matrix& m)
	{
		Gdiplus::Matrix mm(m.a, m.b, m.c, m.d, m.tx, m.ty);
		gdi_->MultiplyTransform(&mm);
	}

	void RenderContext::DrawImage(ImagePart* clip, const Rect& dest_rect)
	{
		ImageFile* image = clip->image();
		if (!image)
			return;
		DrawImage(image, clip->rect(), dest_rect);
	}

	void RenderContext::DrawImage(ImageFile* image, const Rect& src_rect, const Rect& dest_rect)
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

	void RenderContext::DrawStringRect(const std::wstring& text, const Font& font, Color color, const Rect& rect)
	{
		DrawStringRectWithFlags(text, font, color, rect, TEXT_LEFT | TEXT_VCENTER | TEXT_END_ELLIPSIS);
	}

	void RenderContext::DrawStringRectWithFlags(const std::wstring& text, const Font& font, Color color, const Rect& rect, int flags)
	{
		DrawStringRectWithFlags(text.c_str(), text.size(), font, color, rect, flags);
	}

	void RenderContext::DrawStringRectWithFlags(const wchar_t* text, size_t len, const Font& font, Color color, const Rect& rect, int flags)
	{
		Gdiplus::Graphics graphics(dc_);
		Gdiplus::SolidBrush  brush(Gdiplus::Color((unsigned int)color));
		Gdiplus::Font        gdi_font(dc_, font.ToHFONT());
		Gdiplus::StringFormat format;
		Gdiplus::RectF       rectF(rect.x(), rect.y(), rect.width(), rect.height());

		GetStringFormat(format, flags);
		graphics.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
		graphics.DrawString(text, len, &gdi_font, rectF, &format, &brush);
	}

	void RenderContext::CalcStringRectWithFlags(const std::wstring& text, const Font& font, const Rect& rect, int flags, Rect& out, size_t* len, int* lines)
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

	void RenderContext::CalcStringSizeWithFlags(const std::wstring& text, const Font& font, const Size& sz, int flags, Size& out, size_t* len /*= NULL*/, int* lines /*= NULL*/)
	{
		CalcStringSizeWithFlags(text.c_str(), text.size(), font, sz, flags, out, len, lines);
	}

	void RenderContext::CalcStringSizeWithFlags(const wchar_t* text, size_t text_len, const Font& font, const Size& sz, int flags, Size& out, size_t* len /*= NULL*/, int* lines /*= NULL*/)
	{
		HDC hdc = GetDC(NULL);
		Gdiplus::Graphics graphics(hdc);
		Gdiplus::Font        gdi_font(hdc, font.ToHFONT());
		Gdiplus::StringFormat format;
		Gdiplus::RectF       rectF(0, 0, sz.width(), sz.height());

		Gdiplus::RectF outF;

		GetStringFormat(format, flags);
		graphics.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
		graphics.MeasureString(text, text_len, &gdi_font, rectF, &format, &outF, (int*)len, lines);

		out.SetSize(outF.Width, outF.Height);
		::ReleaseDC(NULL, hdc);
	}

	void RenderContext::PushClip(const Rect& rect)
	{
		::SaveDC(dc_);
		HRGN hRgn = ::CreateRectRgnIndirect(&rect.ToRECT());
		::ExtSelectClipRgn(dc_, hRgn, RGN_AND);
		::DeleteObject(hRgn);
	}

	void RenderContext::PopClip()
	{
		::RestoreDC(dc_, -1);
	}



	ScopedPainter::ScopedPainter(RenderContext* painter, const Matrix& m)
		: p_(painter), m_(m.Invert())
	{
		p_->Trans(m);
	}

	ScopedPainter::~ScopedPainter()
	{
		p_->Trans(m_);
	}


	ScopedClipper::ScopedClipper(RenderContext* painter, const Rect& r)
		: p_(painter)
	{
		p_->PushClip(r);
	}

	ScopedClipper::~ScopedClipper()
	{
		p_->PopClip();
	}
}