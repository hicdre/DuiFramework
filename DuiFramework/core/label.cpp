#include "stdafx.h"
#include "label.h"

namespace ui
{

	Label::Label()
	{

	}

	Label::Label(const std::wstring& text)
		: text_(text)
	{

	}

	Label::~Label()
	{

	}

	void Label::SetText(const std::wstring& text)
	{
		text_ = text;
		SchedulePaint();
	}

	void Label::SetMultiLine(bool multi_line)
	{
		is_multi_line_ = multi_line;
	}

	void Label::SetFont(const Font& font)
	{
		font_ = font;
	}

	void Label::SetFont(const std::wstring& name, int size)
	{
		font_ = Font(name, size);
	}

	void Label::SetTextColor(Color color)
	{
		text_color_ = color;
	}

	void Label::OnPaint(Painter* painter)
	{
		Rect rect(GetLocalBounds());
		Border* b = border();
		if (b) {
			rect.Inset(b->GetPadding());
		}
		painter->DrawStringRectWithFlags(text_.c_str(), font_, text_color_, rect, GetTextStyle());
	}

	int Label::GetTextStyle() const
	{
		int flags = TEXT_END_ELLIPSIS;
		if (horizontal_alignment_ == ALIGN_LEFT)
			flags |= TEXT_LEFT;
		else if (horizontal_alignment_ == ALIGN_CENTER)
			flags |= TEXT_CENTER;
		else if (horizontal_alignment_ == ALIGN_RIGHT)
			flags |= TEXT_RIGHT;

		if (horizontal_alignment_ == ALIGN_TOP)
			flags |= TEXT_TOP;
		else if (horizontal_alignment_ == ALIGN_VCENTER)
			flags |= TEXT_VCENTER;
		else if (horizontal_alignment_ == ALIGN_BOTTOM)
			flags |= TEXT_BOTTOM;

		return flags;
	}

}