#include "stdafx.h"
#include "property_object.h"

namespace ui
{
	PropertyObject::PropertyObject()
	{

	}

	PropertyObject::~PropertyObject()
	{

	}

	void PropertyObject::SetPropertyBoolean(const std::string& path, bool in_value)
	{
		if (!property_.get())
			property_.reset(new DictionaryValue);

		property_->SetBoolean(path, in_value);
	}

	void PropertyObject::SetPropertyInteger(const std::string& path, int in_value)
	{
		if (!property_.get())
			property_.reset(new DictionaryValue);

		property_->SetInteger(path, in_value);
	}

	void PropertyObject::SetPropertyDouble(const std::string& path, double in_value)
	{
		if (!property_.get())
			property_.reset(new DictionaryValue);

		property_->SetDouble(path, in_value);
	}

	void PropertyObject::SetPropertyString(const std::string& path, const std::string& in_value)
	{
		if (!property_.get())
			property_.reset(new DictionaryValue);

		property_->SetString(path, in_value);
	}

	void PropertyObject::SetPropertyString(const std::string& path, const std::wstring& in_value)
	{
		if (!property_.get())
			property_.reset(new DictionaryValue);

		property_->SetString(path, in_value);
	}

	bool PropertyObject::GetPropertyBoolean(const std::string& path, bool* out_value) const
	{
		if (!property_.get())
			return false;

		return property_->GetBoolean(path, out_value);
	}

	bool PropertyObject::GetPropertyInteger(const std::string& path, int* out_value) const
	{
		if (!property_.get())
			return false;

		return property_->GetInteger(path, out_value);
	}

	bool PropertyObject::GetPropertyDouble(const std::string& path, double* out_value) const
	{
		if (!property_.get())
			return false;

		return property_->GetDouble(path, out_value);
	}

	bool PropertyObject::GetPropertyString(const std::string& path, std::string* out_value) const
	{
		if (!property_.get())
			return false;

		return property_->GetString(path, out_value);
	}

	bool PropertyObject::GetPropertyString(const std::string& path, std::wstring* out_value) const
	{
		if (!property_.get())
			return false;

		return property_->GetString(path, out_value);
	}

	

}