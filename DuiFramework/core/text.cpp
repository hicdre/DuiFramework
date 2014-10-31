#include "stdafx.h"
#include "text.h"
#include "core/constants.h"

namespace ui
{


	TextFragment::TextFragment()
	{

	}

	TextFragment::TextFragment(const wchar_t* text, size_t len)
		: text_(text)
		, len_(len)
	{

	}

	TextFragment::~TextFragment()
	{

	}

	void TextFragment::SetText(const wchar_t* text, size_t len)
	{
		text_ = text;
		len_ = len;
	}

	void TextFragment::SetFont(const Font& font)
	{
		font_ = font;
	}

	void TextFragment::SetFont(const std::wstring& name, int size)
	{
		font_ = Font(name, size);
	}

	void TextFragment::SetTextColor(Color color)
	{
		text_color_ = color;
	}

	Size TextFragment::GetTextSize() const
	{
		return GetTextSize(text_, len_);
	}

	Size TextFragment::GetTextSize(const wchar_t* text, size_t len) const
	{
		int w = (std::numeric_limits<int>::max)();
		int h = font_.GetHeight();
		Size sz(w, h);
		RenderContext::CalcStringSizeWithFlags(text, len, font_, sz, TEXT_LEFT | TEXT_TOP, sz);
		return sz;
	}

	void TextFragment::DoPaint(RenderContext* painter, const Rect& dest)
	{
		painter->DrawStringRectWithFlags(text_, len_, font_, text_color_, dest, TEXT_LEFT | TEXT_TOP);
	}


}