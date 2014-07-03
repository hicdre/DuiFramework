#pragma once
#include "core/view.h"
#include "core/constants.h"
#include <string>

namespace ui
{
	class Label : public View
	{
	public:
		Label();
		Label(const std::wstring& text);
		virtual ~Label();

		void SetText(const std::wstring& text);
		std::wstring text() const { return text_; };

		bool is_multi_line() const { return is_multi_line_; }
		void SetMultiLine(bool multi_line);

		void SetFont(const Font& font);
		void SetFont(const std::wstring& name, int size);
		
		void SetTextColor(Color color);

		void SetHorizontalAlignment(HorizontalAlignment i) { horizontal_alignment_ = i; }
		HorizontalAlignment GetHorizontalAlignment() const { return horizontal_alignment_; }

		void SetVerticalAlignment(VerticalAlignment i) { vertical_alignment_ = i; }
		VerticalAlignment GetVerticalAlignment() const { return vertical_alignment_; }
	protected:
		virtual void OnPaint(Painter* painter) override;

	private:
		int GetTextStyle() const;
		std::wstring text_;
		bool is_multi_line_{ false };

		Font font_;
		Color text_color_{ 0 };
		HorizontalAlignment horizontal_alignment_{ ALIGN_LEFT };
		VerticalAlignment vertical_alignment_{ ALIGN_TOP };
	};
}