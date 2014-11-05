#include "stdafx.h"
#include "style_value.h"

namespace ui
{
	StyleValue::StyleValue(StyleValueType t /*= StyleValue_Null*/)
		: type_(t), int_value_(0)
	{

	}

	StyleValue::StyleValue(int32 val, StyleValueType t)
	{
		SetIntValue(val, t);
	}

	StyleValue::StyleValue(float val, StyleValueType t)
	{
		SetFloatValue(val, t);
	}

	StyleValue::StyleValue(const std::string& val, StyleValueType t)
	{
		SetStringValue(val, t);
	}

	StyleValue::~StyleValue()
	{
		if (type_ != StyleValue_Null)
			Reset();
	}

	ui::StyleValueType StyleValue::GetType() const
	{
		return type_;
	}

	bool StyleValue::IsNull() const
	{
		return type_ == StyleValue_Null;
	}

	bool StyleValue::IsIntValue() const
	{
		return type_ == StyleValue_Interger;
	}

	bool StyleValue::IsAutoValue() const
	{
		return type_ == StyleValue_Auto;
	}


	bool StyleValue::IsStringValue() const
	{
		return type_ == StyleValue_String;
	}


	bool StyleValue::IsArrayValue() const
	{
		return type_ == StyleValue_Array;
	}


	int32 StyleValue::GetIntValue() const
	{
		assert(IsIntValue());
		return int_value_;
	}



	float StyleValue::GetPercentValue() const
	{
		assert(type_ == StyleValue_Percent);
		return float_value_;
	}

	const std::string& StyleValue::GetStringValue() const
	{
		assert(type_ == StyleValue_String && string_value_);
		return string_value_->Get();
	}

	Color StyleValue::GetColorValue() const
	{
		assert(type_ == StyleValue_Color);
		return color_value_;
	}

	void StyleValue::SetAutoValue()
	{
		Reset();
		type_ = StyleValue_Auto;
	}

	void StyleValue::SetIntValue(int32 value, StyleValueType type)
	{
		Reset();
		type_ = type;
		int_value_ = value;
	}

	void StyleValue::SetPercentValue(float value)
	{
		SetFloatValue(value, StyleValue_Percent);
		
	}

	void StyleValue::SetFloatValue(float value, StyleValueType type)
	{
		type_ = type;
		float_value_ = value;
	}

	void StyleValue::SetColorValue(Color value)
	{
		type_ = StyleValue_Color;
		color_value_ = value;
	}


	void StyleValue::SetStringValue(const std::string& val, StyleValueType type)
	{
		if (IsStringValue())
		{
			type_ = type;
			string_value_->Set(val);
		}
		else
		{
			Reset();
			type_ = type;
			string_value_ = new StyleValueString(val);
		}
	}


	void StyleValue::Reset()
	{
		if (type_ == StyleValue_String)
		{
			if (string_value_)
				delete string_value_;
			string_value_ = NULL;
		}
		else if (type_ == StyleValue_Array)
		{
			if (array_value_)
				delete array_value_;
			array_value_ = NULL;
		}
		else
		{
			int_value_ = 0;
		}
	}

	bool StyleValue::IsEqual(StyleValue* val) const
	{
		if (type_ != val->type_)
			return false;

		if (type_ == StyleValue_String)
			return string_value_->IsEqual(val->string_value_);

		if (type_ == StyleValue_Array)
			return array_value_->IsEqual(val->array_value_);

		return int_value_ == val->int_value_;
	}


	void StyleValueString::Set(const std::string& val)
	{
		str_ = val;
	}

	const std::string& StyleValueString::Get() const
	{
		return str_;
	}

	bool StyleValueString::IsEqual(StyleValueString* val)
	{
		return str_.compare(val->str_) == 0;
	}

	StyleValueString::~StyleValueString()
	{

	}

	StyleValueString::StyleValueString(const std::string& str) : str_(str)
	{

	}


	bool StyleValueArray::IsEqual(StyleValueArray* val) const
	{
		if (val->GetCount() != GetCount())
			return false;
		int count = GetCount();
		for (int i = 0; i < count; i++)
		{
			if (!Get(i)->IsEqual(val->Get(i)))
				return false;
		}
		return true;
	}

	void StyleValueArray::Remove(StyleValue* val)
	{
		for (auto iter = array_.begin(); iter != array_.end(); iter++)
		{
			StyleValue* v = *iter;
			if (v->IsEqual(val))
			{
				v->Release();
				array_.erase(iter);
				break;
			}
		}
	}

	void StyleValueArray::Add(StyleValue* val)
	{
		val->AddRef();
		array_.push_back(val);
	}

	void StyleValueArray::Reset()
	{
		for (StyleValue* v : array_)
		{
			v->Release();
		}
		array_.clear();
	}

	StyleValue* StyleValueArray::Get(int index) const
	{
		return array_.at(index);
	}

	int StyleValueArray::GetCount() const
	{
		return array_.size();
	}

	StyleValueArray::~StyleValueArray()
	{
		Reset();
	}

	StyleValueArray::StyleValueArray(StyleValue* val)
	{
		Add(val);
	}

	StyleValueArray::StyleValueArray()
	{

	}

}