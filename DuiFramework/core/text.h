#pragma once
#include "core/drawable.h"

namespace ui
{
	class TextFragment : public Drawable
	{
	public:
		TextFragment();
		TextFragment(const wchar_t* text, size_t len);
		virtual ~TextFragment();

		void SetText(const wchar_t* text, size_t len);

		void SetFont(const Font& font);
		void SetFont(const std::wstring& name, int size);

		void SetTextColor(Color color);

		Size GetTextSize() const;
		Size GetTextSize(const wchar_t* text, size_t len) const;

		size_t length() const { return len_; }

		virtual void DoPaint(Painter* painter, const Rect& dest) override;
	private:
		const wchar_t* text_;
		size_t len_;
		Font font_;
		Color text_color_{ 0 };
	};

}