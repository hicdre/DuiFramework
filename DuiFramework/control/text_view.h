#pragma once
#include "core/view.h"
#include "core/text.h"

namespace ui
{
	class TextView : public View
	{
	public:
		TextView();
		TextView(const std::wstring& text);
		virtual ~TextView();

		void SetText(const std::wstring& text);
		std::wstring text() const { return text_; };

		void SetEndEllipsis(bool v);
		bool GetEndEllipsis();

		void SetFont(const Font& font);
		void SetFont(const std::wstring& name, int size);

		void SetTextColor(Color color);

		void SetHorizontalAlignment(HorizontalAlignment i) { horizontal_alignment_ = i; }
		HorizontalAlignment GetHorizontalAlignment() const { return horizontal_alignment_; }

		void SetVerticalAlignment(VerticalAlignment i) { vertical_alignment_ = i; }
		VerticalAlignment GetVerticalAlignment() const { return vertical_alignment_; }
	protected:
		virtual void OnPaint(RenderContext* painter) override;
		virtual void Layout() override;

	private:
		void BuildData();

		std::wstring text_;
		bool is_end_ellipsis_{ false };

		bool needs_build_{ true };
		TextFragment text_fragment_;
		Rect text_rect_;

		Font font_;
		Color text_color_{ 0 };
		HorizontalAlignment horizontal_alignment_{ ALIGN_LEFT };
		VerticalAlignment vertical_alignment_{ ALIGN_TOP };
	};
}