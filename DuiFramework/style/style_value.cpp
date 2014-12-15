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
		return IsIntType(type_);
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
		return GetFloatValue();
	}

	float StyleValue::GetFloatValue() const
	{
		assert(IsFloatValue());
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

	void StyleValue::SetIntValue(int32 value)
	{
		SetIntValue(value, StyleValue_Integer);
	}

	void StyleValue::SetPercentValue(float value)
	{
		SetFloatValueInternal(value, StyleValue_Percent);
	}

	void StyleValue::SetFloatValueInternal(float value, StyleValueType type)
	{
		Reset();
		type_ = type;
		float_value_ = value;
	}

	void StyleValue::SetFloatValue(float value)
	{
		SetFloatValueInternal(value, StyleValue_Float);
	}

	void StyleValue::SetFloatValue(float value, StyleValueType type)
	{
		assert(IsFloatType(type));
		SetFloatValueInternal(value, type);
	}

	void StyleValue::SetColorValue(Color value)
	{
		type_ = StyleValue_Color;
		color_value_ = value;
	}


	void StyleValue::SetStringValueInternal(const std::string& val, StyleValueType type)
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

	void StyleValue::SetStringValue(const std::string& value)
	{
		SetStringValueInternal(value, StyleValue_String);
	}

	void StyleValue::SetStringValue(const std::string& val, StyleValueType type)
	{
		assert(IsStringType(type));
		SetStringValueInternal(val, type);
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
		else if (type_ == StyleValue_URL)
		{
			if (url_value_)
				delete url_value_;
			url_value_ = NULL;
		}
		else if (type_ == StyleValue_Function)
		{
			if (function_value_)
				delete function_value_;
			function_value_ = NULL;
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

		if (type_ == StyleValue_URL)
			return url_value_->IsEquals(*val->url_value_);

		if (type_ == StyleValue_Function)
			return function_value_->IsEqual(val->function_value_);

		return int_value_ == val->int_value_;
	}

	int32 StyleValue::GetPixel() const
	{
		assert(IsPixelValue());
		return (int32)float_value_;
	}

	bool StyleValue::IsColorValue() const
	{
		return type_ == StyleValue_Color;
	}

	void StyleValue::SetArrayValue(StyleValueArray* value)
	{
		Reset();
		type_ = StyleValue_Array;
		array_value_ = value;
	}

	StyleValueArray* StyleValue::GetArrayValue() const
	{
		assert(type_ == StyleValue_Array);
		return array_value_;
	}

	bool StyleValue::IsCursorValue() const
	{
		return type_ == StyleValue_Cursor;
	}

	void StyleValue::SetCursorValue(CursorId id)
	{
		SetIntValue(id, StyleValue_Cursor);
	}

	ui::CursorId StyleValue::GetCursorValue() const
	{
		assert(type_ == StyleValue_Cursor);
		return (CursorId)GetIntValue();
	}

	bool StyleValue::IsIntType(StyleValueType t)
	{
		return t == StyleValue_Integer 
			|| t == StyleValue_Cursor;
	}

	bool StyleValue::IsFloatValue() const
	{
		return IsFloatType(type_);
	}

	bool StyleValue::IsFloatType(StyleValueType t)
	{
		return t == StyleValue_Float
			|| t == StyleValue_Pixel
			|| t == StyleValue_Percent
			|| t == StyleValue_Number
			|| t == StyleValue_Degree;
	}

	bool StyleValue::IsPixelValue() const
	{
		return type_ == StyleValue_Pixel;
	}

	bool StyleValue::IsPercentValue() const
	{
		return type_ == StyleValue_Percent;
	}

	void StyleValue::SetNumberValue(float value)
	{
		SetIntValue(value, StyleValue_Number);
	}

	bool StyleValue::IsStringType(StyleValueType t)
	{
		return t == StyleValue_String
			|| t == StyleValue_ResourceId;
	}

	bool StyleValue::IsUrlValue() const
	{
		return type_ == StyleValue_URL;
	}

	const URL& StyleValue::GetUrlValue() const
	{
		assert(IsUrlValue());
		return *url_value_;
	}

	void StyleValue::SetUrlValue(const std::string& str)
	{
		SetUrlValue(URL(str));
	}


	void StyleValue::SetUrlValue(const std::wstring& str)
	{
		SetUrlValue(URL(str));
	}

	void StyleValue::SetUrlValue(const URL& url)
	{
		Reset();
		type_ = StyleValue_URL;
		url_value_ = new URL(url);
	}

	bool StyleValue::IsFunctionValue() const
	{
		return type_ == StyleValue_Function;
	}

	StyleValueFunction* StyleValue::GetFunctionValue() const
	{
		assert(IsFunctionValue());
		return function_value_;
	}

	void StyleValue::SetFunctionValue(const std::string& name)
	{
		Reset();
		type_ = StyleValue_Function;
		function_value_ = new StyleValueFunction(name);
	}

	void StyleValue::SetFunctionValue(StyleValueFunction* value)
	{
		Reset();
		type_ = StyleValue_Function;
		function_value_ = value;
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




	StyleValueFunction::StyleValueFunction(const std::string& name)
		: name_(name)
	{

	}

	StyleValueFunction::~StyleValueFunction()
	{
		for (StyleValue* v : params_)
		{
			v->Release();
		}
		params_.clear();
	}

	StyleValue* StyleValueFunction::GetParam(size_t index) const
	{
		if (index < params_.size())
			return params_[index];
		return NULL;
	}

	void StyleValueFunction::AddParam(StyleValue* val)
	{
		params_.push_back(val);
		val->AddRef();
	}

	bool StyleValueFunction::IsEqual(StyleValueFunction* val) const
	{
		if (val->name_ != name_)
			return false;
		if (val->GetParamsCount() != GetParamsCount())
			return false;
		int count = GetParamsCount();
		for (int i = 0; i < count; i++)
		{
			if (!GetParam(i)->IsEqual(val->GetParam(i)))
				return false;
		}
		return true;
	}


}