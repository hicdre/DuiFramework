#include "stdafx.h"
#include "view_layout.h"

namespace ui
{

	Length::Length()
		: type_(Auto)
		, int_value_(0)
		, is_float_(false)
	{

	}

	Length::Length(LengthType type)
		: type_(type)
		, int_value_(0)
		, is_float_(false)
	{

	}

	Length::Length(int v, LengthType t)
		: type_(t)
		, int_value_(v)
		, is_float_(false)
	{

	}

	Length::Length(float v, LengthType t)
		: type_(t)
		, float_value_(v)
		, is_float_(true)
	{

	}

	bool Length::IsAuto() const
	{
		return type_ == Auto;
	}

	bool Length::IsPercent() const
	{
		return type_ == Percent;
	}

	bool Length::IsFixed() const
	{
		return type_ == Fixed;
	}

	float Length::percent() const
	{
		assert(IsPercent());
		return float_value_;
	}

	int Length::intValue() const
	{
		return is_float_ ? static_cast<int>(float_value_) : int_value_;
	}

	float Length::value() const
	{
		return is_float_ ? float_value_ : static_cast<float>(int_value_);
	}

	void Length::setValue(LengthType t, int value)
	{
		type_ = t;
		float_value_ = value;
		is_float_ = false;
	}

	void Length::setValue(LengthType t, float value)
	{
		type_ = t;
		float_value_ = value;
		is_float_ = true;
	}

	void Length::setValue(float value)
	{
		*this = Length(value, Fixed);
	}

	ui::LengthType Length::type() const
	{
		return type_;
	}

	ui::Length Length::FromString(const std::string& str)
	{
		const char* p = str.c_str();
		char* end = NULL;
		float f = strtof(p, &end);

		while (*end && isspace(*end))
			end++;

		if (*end == 0)
			return Length(f, Fixed);
		else if (*end == '%')
			return Length((float)(f/100.0), Percent);
		else
			return Length();
	}


	LayoutBox::LayoutBox()
	{

	}

	LayoutBox::~LayoutBox()
	{

	}

	const Length& LayoutBox::x()
	{
		return margin_left_;
	}

	const Length& LayoutBox::y()
	{
		return margin_top_;
	}

	const Length& LayoutBox::width()
	{
		return width_;
	}

	const Length& LayoutBox::height()
	{
		return height_;
	}

	const Length& LayoutBox::marginLeft()
	{
		return margin_left_;
	}

	const Length& LayoutBox::marginTop()
	{
		return margin_top_;
	}

	const Length& LayoutBox::marginRight()
	{
		return margin_right_;
	}

	const Length& LayoutBox::marginBottom()
	{
		return margin_bottom_;
	}

	void LayoutBox::setX(const Length& l)
	{
		margin_left_ = l;
	}

	void LayoutBox::setY(const Length& l)
	{
		margin_top_ = l;
	}

	void LayoutBox::setWidth(const Length& l)
	{
		width_ = l;
	}

	void LayoutBox::setHeight(const Length& l)
	{
		height_ = l;
	}

	void LayoutBox::setMarginLeft(const Length& l)
	{
		margin_left_ = l;
	}

	void LayoutBox::setMarginTop(const Length& l)
	{
		margin_top_ = l;
	}

	void LayoutBox::setMarginRight(const Length& l)
	{
		margin_right_ = l;
	}

	void LayoutBox::setMarginBottom(const Length& l)
	{
		margin_bottom_ = l;
	}

}