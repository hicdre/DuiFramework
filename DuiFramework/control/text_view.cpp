#include "stdafx.h"
#include "text_view.h"

namespace ui
{


	TextView::TextView()
	{

	}

	TextView::TextView(const std::wstring& text)
		: text_(text)
	{

	}

	TextView::~TextView()
	{

	}

	void TextView::SetText(const std::wstring& text)
	{
		text_ = text;
		needs_build_ = true;
		Layout();
		SchedulePaint();
	}

	void TextView::SetEndEllipsis(bool v)
	{
		if (is_end_ellipsis_ == v)
			return;
		is_end_ellipsis_ = v;
		needs_build_ = true;
		Layout();
		SchedulePaint();
	}

	bool TextView::GetEndEllipsis()
	{
		return is_end_ellipsis_;
	}

	void TextView::SetFont(const Font& font)
	{
		text_fragment_.SetFont(font);
	}

	void TextView::SetFont(const std::wstring& name, int size)
	{
		text_fragment_.SetFont(name, size);
	}

	void TextView::SetTextColor(Color color)
	{
		text_fragment_.SetTextColor(color);
	}

	void TextView::OnPaint(RenderContext* painter)
	{
		BuildData();
		text_fragment_.DoPaint(painter, text_rect_);
	}


	void TextView::Layout()
	{
		BuildData();

		Rect rect(GetContentsBounds());
		Size sz = text_fragment_.GetTextSize();
		int x, y;
		if (horizontal_alignment_ == ALIGN_LEFT) {
			x = rect.x();
		}
		else if (horizontal_alignment_ == ALIGN_CENTER) {
			x = rect.x() + (rect.width() - sz.width()) / 2;
		}
		else { // ALIGN_RIGHT
			x = rect.right() - sz.width();
		}

		if (vertical_alignment_ == ALIGN_TOP) {
			y = rect.y();
		}
		else if (horizontal_alignment_ == ALIGN_VCENTER) {
			y = rect.y() + (rect.height() - sz.height()) / 2;
		}
		else { // ALIGN_BOTTOM
			y = rect.bottom() - sz.height();
		}

		text_rect_.SetRect(x, y, sz.width(), sz.height());
	}


	void TextView::BuildData()
	{
		if (!needs_build_)
			return;

		Rect rect(GetContentsBounds());
		if (rect.IsEmpty())
			return;
		//TODO: Ê¹ÓÃUniscribeÖØÐ´
		size_t text_len = text_.size();
		Size full_sz = text_fragment_.GetTextSize(text_.c_str(), text_len);

		if (full_sz.width() > rect.width())
		{
			size_t low_bounds = 0;
			size_t high_bounds = text_.size();
			
			while (true)
			{
				text_len = (low_bounds + high_bounds) / 2;
				Size text_size = text_fragment_.GetTextSize(text_.c_str(), text_len);
				if (text_size.width() > rect.width())
					high_bounds = text_len;
				else if (text_size.width() < rect.width())
					low_bounds = text_len;

				if (low_bounds + 1 == high_bounds) {
					text_len = low_bounds;
					break;
				}
			}
			
		}
		text_fragment_.SetText(text_.c_str(), text_len);
		needs_build_ = false;
	}

}