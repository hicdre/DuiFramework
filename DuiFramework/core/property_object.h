#pragma once
#include "base/basictypes.h"
#include "base/scoped_ptr.h"
#include "core/value.h"

namespace ui
{
	class PropertyObject
	{
	public:
		PropertyObject();
		virtual ~PropertyObject();

		// Property-------------------------------------------
		void SetPropertyBoolean(const std::string& path, bool in_value);
		void SetPropertyInteger(const std::string& path, int in_value);
		void SetPropertyDouble(const std::string& path, double in_value);
		void SetPropertyString(const std::string& path, const std::string& in_value);
		void SetPropertyString(const std::string& path, const std::wstring& in_value);

		bool GetPropertyBoolean(const std::string& path, bool* out_value) const;
		bool GetPropertyInteger(const std::string& path, int* out_value) const;
		bool GetPropertyDouble(const std::string& path, double* out_value) const;
		bool GetPropertyString(const std::string& path, std::string* out_value) const;
		bool GetPropertyString(const std::string& path, std::wstring* out_value) const;

	protected:
		scoped_ptr<DictionaryValue> property_;
	};
}

