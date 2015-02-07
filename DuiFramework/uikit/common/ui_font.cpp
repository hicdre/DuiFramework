#include "stdafx.h"
#include "ui_font.h"

namespace ui
{

	UIFont::UIFont(const std::wstring& font_name, int font_size)
		: font_name_(font_name)
		, font_size_(font_size)
		, style_(0)
	{
	}

	UIFont::UIFont(const std::wstring& font_name,
		int font_size,
		int style)
		: font_name_(font_name)
		, font_size_(font_size)
		, style_(style)
	{
	}

	UIFont::~UIFont()
	{
	}


	UIFont* UIFont::Derive(int size_delta, int style) const
	{
		return new UIFont(font_name_
			, font_size_ + size_delta
			, style);
	}

	bool UIFont::IsEqual(UIFont* other) const
	{
		return other->font_size_ == font_size_
			&& other->style_ == style_
			&& other->font_name_ == font_name_;
	}

	std::wstring UIFont::description() const
	{
		return StringPrintf(L"[UIFont:%s, size:%d, style:%d]",
			font_name_.c_str(), font_size_, style_);
	}

/////////////////////////////////////////////////////////////////////////
	namespace {

		const int kTextMetricWeightBold = 700;

		void GetTextMetricsForFont(HDC hdc, HFONT font, TEXTMETRIC* text_metrics) {
			HGDIOBJ hprev = ::SelectObject(hdc, font);
			::GetTextMetrics(hdc, text_metrics);
			::SelectObject(hdc, hprev);
		}

	}


	class UIFontFactory
	{
	public:
		UIFontFactory()
		{
			NONCLIENTMETRICS metrics;
			static const UINT SIZEOF_NONCLIENTMETRICS = sizeof(NONCLIENTMETRICS);
			metrics.cbSize = sizeof(NONCLIENTMETRICS);
			SystemParametersInfo(SPI_GETNONCLIENTMETRICS,
				SIZEOF_NONCLIENTMETRICS, &metrics,
				0);

			metrics.lfMessageFont.lfHeight =
				AdjustFontSize(metrics.lfMessageFont.lfHeight, 0);
			HFONT font = CreateFontIndirect(&metrics.lfMessageFont);
			font_ = CreateFontInteral(font);
		}

		~UIFontFactory()
		{
			if (font_)
			{
				font_->Release();
				font_ = NULL;
			}
		}

		static UIFont* GetDefaultFont()
		{
			static UIFontFactory factory;
			return factory.font_;
		}
	private:
		int AdjustFontSize(int lf_height, int size_delta) {
			if (lf_height < 0) {
				lf_height -= size_delta;
			}
			else {
				lf_height += size_delta;
			}
			return lf_height;
		}

		UIFont* CreateFontInteral(HFONT font)
		{
			TEXTMETRIC font_metrics;
			{
				HDC screen_dc = ::GetDC(NULL);
				int old = ::SetMapMode(screen_dc, MM_TEXT);
				GetTextMetricsForFont(screen_dc, font, &font_metrics);
				::SetMapMode(screen_dc, old);
				::ReleaseDC(NULL, screen_dc);
			}

			return CreateFontInteral(font, font_metrics);
		}

		UIFont* CreateFontInteral(HFONT font, const TEXTMETRIC& font_metrics)
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

			LOGFONT font_info;
			GetObject(font, sizeof(LOGFONT), &font_info);

			::DeleteObject(font);

			return new UIFont(font_info.lfFaceName, font_size, style);
		}

		UIFont* font_;

	};

	scoped_refptr<UIFont> UIFont::CreateDefault()
	{
		return UIFontFactory::GetDefaultFont();
	}

	scoped_refptr<UIFont> UIFont::CreateFont(const std::wstring& font_name,
		int font_size,
		int style)
	{
		return new UIFont(font_name, font_size, style);
	}
}