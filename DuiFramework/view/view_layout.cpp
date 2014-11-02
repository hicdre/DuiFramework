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

	bool Length::IsAuto()
	{
		return type_ == Auto;
	}

	bool Length::IsPercent()
	{
		return type_ == Percent;
	}

	bool Length::IsFixed()
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

}