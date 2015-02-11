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
	public:
		void init(const std::wstring& s);
		void init(const wchar_t* str);
		void init(const char* str);
		void initWithFormat(const char* format, ...);
		void initWithFormat(const wchar_t* format, ...);
		void initWithString(UIString* str);

		size_t length() const { return utf16_.size(); }
		wchar_t characterAtIndex(size_t index) const { return utf16_[index]; }

		typedef std::wstring::iterator iterator;
		typedef std::wstring::const_iterator const_iterator;
		typedef std::wstring::reverse_iterator reverse_iterator;
		typedef std::wstring::const_reverse_iterator const_reverse_iterator;
		iterator begin() { return utf16_.begin(); }
		const_iterator begin() const { return utf16_.begin(); }
		iterator end() { return utf16_.end(); }
		const_iterator end() const { return utf16_.end(); }
		reverse_iterator rbegin() { return utf16_.rbegin(); }
		const_reverse_iterator rbegin() const { return utf16_.rbegin(); }
		reverse_iterator rend() { return utf16_.rend(); }
		const_reverse_iterator rend() const { return utf16_.rend(); }


	protected:
		UIString();
		~UIString();
		void initWithFormatV(const char* format, va_list ap);
		void initWithFormatV(const wchar_t* format, va_list ap);

	private:
		friend class RefCounted < UIString > ;
		std::wstring utf16_;
	};
}