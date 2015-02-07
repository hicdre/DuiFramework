#pragma once
#include "foundation/basictypes.h"
#include "foundation/base/ref_counted.h"
#include "foundation/base/scoped_ptr.h"
#include <string>

namespace ui
{
	const int kCP_UTF8 = 65001;       // UTF-8 translation
	std::wstring MultiByteToWide(const std::string& mb, uint32 code_page = kCP_UTF8);
	std::string WideToMultiByte(const std::wstring& wide, uint32 code_page = kCP_UTF8);

	//暂时不考虑ucs4的问题
	class UIString : public RefCounted < UIString >
	{
	public:
		static scoped_refptr<UIString> str();
		static scoped_refptr<UIString> str(const wchar_t* str);
		static scoped_refptr<UIString> str(const char* str);
		static scoped_refptr<UIString> strWithFormat(const char* format, ...);
		static scoped_refptr<UIString> strWithFormat(const wchar_t* format, ...);
		static scoped_refptr<UIString> strWithString(UIString* str);
	protected:
		UIString();
		~UIString();

		void init(const std::wstring& s);

		size_t length() const { return utf16_.size(); }
		wchar_t characterAtIndex(size_t index) { return utf16_[index]; }
	private:
		friend class RefCounted < UIString > ;
		std::wstring utf16_;
	};
}