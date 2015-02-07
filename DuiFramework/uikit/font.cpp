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
			font_info->lfUnderline = (font_style & UIFont::UNDERLINE) != 0;
			font_info->lfItalic = (font_style & UIFont::ITALIC) != 0;
			font_info->lfWeight = (font_style & UIFont::BOLD) ? FW_BOLD : FW_NORMAL;
		}
	}

	UIFont::UIFont()
		: font_ref_(CreateDefalutHFontRef())
	{
		font_ref_->AddRef();
	}


	UIFont::UIFont(const UIFont& other)
	{
		if (font_ref_)
			font_ref_->Release();
		font_ref_ = other.font_ref_;
		font_ref_->AddRef();
	}

	UIFont::UIFont(HFONT hfont)
	{
		InitWithCopyOfHFONT(hfont);
	}

	UIFont::UIFont(const std::wstring& font_name, int font_size)
	{
		InitWithFontNameAndSize(font_name, font_size);
	}

	UIFont::~UIFont()
	{
		if (font_ref_)
			font_ref_->Release();
	}

	UIFont::UIFont(HFontRef* hfont_ref)
		: font_ref_(hfont_ref)
	{
		font_ref_->AddRef();
	}

	UIFont& UIFont::operator=(const UIFont& other)
	{
		if (font_ref_)
			font_ref_->Release();
		font_ref_ = other.font_ref_;
		font_ref_->AddRef();
		return *this;
	}

	UIFont UIFont::Derive(int size_delta, int style) const
	{
		LOGFONT font_info;
		GetObject(ToHFONT(), sizeof(LOGFONT), &font_info);
		const int requested_font_size = font_ref_->requested_font_size();
		font_info.lfHeight = AdjustFontSize(-requested_font_size, size_delta);
		SetLogFontStyle(style, &font_info);

		HFONT hfont = CreateFontIndirect(&font_info);
		return UIFont(CreateHFontRef(hfont));
	}

	int UIFont::GetHeight() const
	{
		return font_ref_->height();
	}

	int UIFont::GetBaseline() const
	{
		return font_ref_->baseline();
	}

	int UIFont::GetCapHeight() const
	{
		return font_ref_->cap_height();
	}

	int UIFont::GetStyle() const
	{
		return font_ref_->style();
	}

	std::wstring UIFont::GetFontName() const
	{
		return font_ref_->font_name();
	}

	int UIFont::GetFontSize() const
	{
		return font_ref_->font_size();
	}

	HFONT UIFont::ToHFONT() const
	{
		return font_ref_->hfont();
	}

	void UIFont::InitWithCopyOfHFONT(HFONT hfont)
	{
		LOGFONT font_info;
		GetObject(hfont, sizeof(LOGFONT), &font_info);
		font_ref_ = CreateHFontRef(CreateFontIndirect(&font_info));
		if (font_ref_)
			font_ref_->AddRef();
	}

	void UIFont::InitWithFontNameAndSize(const std::wstring& font_name, int font_size)
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

	UIFont::HFontRef* UIFont::CreateHFontRef(HFONT font)
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

	UIFont::HFontRef* UIFont::CreateHFontRef(HFONT font, const TEXTMETRIC& font_metrics)
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
			style |= UIFont::ITALIC;
		if (font_metrics.tmUnderlined)
			style |= UIFont::UNDERLINE;
		if (font_metrics.tmWeight >= kTextMetricWeightBold)
			style |= UIFont::BOLD;

		return new HFontRef(font, font_size, height, baseline, cap_height,
			ave_char_width, style);
	}

	UIFont::HFontRef* UIFont::base_font_ref_ = NULL;

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

	void UIFont::UnInitFont()
	{
		if (base_font_ref_)
			base_font_ref_->Release();
	}

	UIFont::HFontRef* UIFont::CreateDefalutHFontRef()
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

	

	UIFont::HFontRef::HFontRef(HFONT hfont, int font_size,
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

	UIFont::HFontRef::~HFontRef()
	{
		DeleteObject(hfont_);
	}

}