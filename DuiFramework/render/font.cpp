#include "stdafx.h"
#include "font.h"

namespace ui
{
	namespace
	{
		const int kTextMetricWeightBold = 700;

		void GetTextMetricsForFont(HDC hdc, HFONT font, TEXTMETRIC* text_metrics) {
			HGDIOBJ hprev = ::SelectObject(hdc, font);
			::GetTextMetrics(hdc, text_metrics);
			::SelectObject(hdc, hprev);
		}

		// Returns either minimum font allowed for a current locale or
		// lf_height + size_delta value.
		int AdjustFontSize(int lf_height, int size_delta) {
			if (lf_height < 0) {
				lf_height -= size_delta;
			}
			else {
				lf_height += size_delta;
			}
				return lf_height;
		}

		// Sets style properties on |font_info| based on |font_style|.
		void SetLogFontStyle(int font_style, LOGFONT* font_info) {
			font_info->lfUnderline = (font_style & Font::UNDERLINE) != 0;
			font_info->lfItalic = (font_style & Font::ITALIC) != 0;
			font_info->lfWeight = (font_style & Font::BOLD) ? FW_BOLD : FW_NORMAL;
		}
	}

	Font::Font()
		: font_ref_(CreateDefalutHFontRef())
	{
	}


	Font::Font(const Font& other)
	{
		font_ref_ = other.font_ref_;
		font_ref_->AddRef();
	}

	Font::Font(HFONT hfont)
	{
		InitWithCopyOfHFONT(hfont);
	}

	Font::Font(const std::wstring& font_name, int font_size)
	{
		InitWithFontNameAndSize(font_name, font_size);
	}

	Font::~Font()
	{
		if (font_ref_)
			font_ref_->Release();
	}

	Font::Font(HFontRef* hfont_ref)
		: font_ref_(hfont_ref)
	{
		font_ref_->AddRef();
	}

	Font& Font::operator=(const Font& other)
	{
		font_ref_ = other.font_ref_;
		font_ref_->AddRef();
		return *this;
	}

	Font Font::Derive(int size_delta, int style) const
	{
		LOGFONT font_info;
		GetObject(ToHFONT(), sizeof(LOGFONT), &font_info);
		const int requested_font_size = font_ref_->requested_font_size();
		font_info.lfHeight = AdjustFontSize(-requested_font_size, size_delta);
		SetLogFontStyle(style, &font_info);

		HFONT hfont = CreateFontIndirect(&font_info);
		return Font(CreateHFontRef(hfont));
	}

	int Font::GetHeight() const
	{
		return font_ref_->height();
	}

	int Font::GetBaseline() const
	{
		return font_ref_->baseline();
	}

	int Font::GetCapHeight() const
	{
		return font_ref_->cap_height();
	}

	int Font::GetStyle() const
	{
		return font_ref_->style();
	}

	std::wstring Font::GetFontName() const
	{
		return font_ref_->font_name();
	}

	int Font::GetFontSize() const
	{
		return font_ref_->font_size();
	}

	HFONT Font::ToHFONT() const
	{
		return font_ref_->hfont();
	}

	void Font::InitWithCopyOfHFONT(HFONT hfont)
	{
		LOGFONT font_info;
		GetObject(hfont, sizeof(LOGFONT), &font_info);
		font_ref_ = CreateHFontRef(CreateFontIndirect(&font_info));
		if (font_ref_)
			font_ref_->AddRef();
	}

	void Font::InitWithFontNameAndSize(const std::wstring& font_name, int font_size)
	{
		HFONT hf = ::CreateFont(-font_size, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE,
			DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_DONTCARE,
			font_name.c_str());
		font_ref_ = CreateHFontRef(hf);
		if (font_ref_)
			font_ref_->AddRef();
	}

	Font::HFontRef* Font::CreateHFontRef(HFONT font)
	{
		TEXTMETRIC font_metrics;
		{
			HDC screen_dc = ::GetDC(NULL);
			int old = ::SetMapMode(screen_dc, MM_TEXT);
			GetTextMetricsForFont(screen_dc, font, &font_metrics);
			::SetMapMode(screen_dc, old);
			::ReleaseDC(NULL, screen_dc);
		}

		return CreateHFontRef(font, font_metrics);
	}

	Font::HFontRef* Font::CreateHFontRef(HFONT font, const TEXTMETRIC& font_metrics)
	{
		const int height = std::max<int>(1, font_metrics.tmHeight);
		const int baseline = std::max<int>(1, font_metrics.tmAscent);
		const int cap_height =
			std::max<int>(1, font_metrics.tmAscent - font_metrics.tmInternalLeading);
		const int ave_char_width = std::max<int>(1, font_metrics.tmAveCharWidth);
		const int font_size =
			std::max<int>(1, font_metrics.tmHeight - font_metrics.tmInternalLeading);
		int style = 0;
		if (font_metrics.tmItalic)
			style |= Font::ITALIC;
		if (font_metrics.tmUnderlined)
			style |= Font::UNDERLINE;
		if (font_metrics.tmWeight >= kTextMetricWeightBold)
			style |= Font::BOLD;

		return new HFontRef(font, font_size, height, baseline, cap_height,
			ave_char_width, style);
	}

	Font::HFontRef* Font::base_font_ref_ = NULL;

	/*void Font::InitFont(const std::wstring& font_name, int font_size)
	{

		HFONT hf = ::CreateFont(-font_size, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE,
			DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_DONTCARE,
			font_name.c_str());
		base_font_ref_ = CreateHFontRef(hf);
		if (base_font_ref_)
			base_font_ref_->AddRef();
	}*/

	void Font::UnInitFont()
	{
		if (base_font_ref_)
			base_font_ref_->Release();
	}

	Font::HFontRef* Font::CreateDefalutHFontRef()
	{
		if (!base_font_ref_)
		{
			NONCLIENTMETRICS metrics;
			static const UINT SIZEOF_NONCLIENTMETRICS =	sizeof(NONCLIENTMETRICS);
			metrics.cbSize = sizeof(NONCLIENTMETRICS);
			SystemParametersInfo(SPI_GETNONCLIENTMETRICS,
				SIZEOF_NONCLIENTMETRICS, &metrics,
				0);
			metrics.lfMessageFont.lfHeight =
				AdjustFontSize(metrics.lfMessageFont.lfHeight, 0);
			HFONT font = CreateFontIndirect(&metrics.lfMessageFont);
			base_font_ref_ = CreateHFontRef(font);
			base_font_ref_->AddRef();
		}
		return base_font_ref_;
	}

	

	Font::HFontRef::HFontRef(HFONT hfont, int font_size,
		int height,
		int baseline,
		int cap_height,
		int ave_char_width,
		int style)
		: hfont_(hfont),
		font_size_(font_size),
		height_(height),
		baseline_(baseline),
		cap_height_(cap_height),
		ave_char_width_(ave_char_width),
		style_(style),
		//dlu_base_x_(-1),
		requested_font_size_(font_size)
	{
		LOGFONT font_info;
		GetObject(hfont_, sizeof(LOGFONT), &font_info);
		font_name_ = font_info.lfFaceName;
		if (font_info.lfHeight < 0)
			requested_font_size_ = -font_info.lfHeight;
	}

	Font::HFontRef::~HFontRef()
	{
		DeleteObject(hfont_);
	}

}