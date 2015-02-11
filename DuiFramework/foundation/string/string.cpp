#include "stdafx.h"
#include "string.h"
#include "string_printf.h"

namespace ui
{
	std::wstring MultiByteToWide(const std::string& mb, uint32 code_page /*= CP_UTF8*/)
	{
		if (mb.empty())
			return std::wstring();

		int mb_length = static_cast<int>(mb.length());
		// Compute the length of the buffer.
		int charcount = MultiByteToWideChar(code_page, 0,
			mb.data(), mb_length, NULL, 0);
		if (charcount == 0)
			return std::wstring();

		std::wstring wide;
		wide.resize(charcount);
		MultiByteToWideChar(code_page, 0, mb.data(), mb_length, &wide[0], charcount);

		return wide;
	}

	std::string WideToMultiByte(const std::wstring& wide, uint32 code_page /*= CP_UTF8*/)
	{
		int wide_length = static_cast<int>(wide.length());
		if (wide_length == 0)
			return std::string();

		// Compute the length of the buffer we'll need.
		int charcount = WideCharToMultiByte(code_page, 0, wide.data(), wide_length,
			NULL, 0, NULL, NULL);
		if (charcount == 0)
			return std::string();

		std::string mb;
		mb.resize(charcount);
		WideCharToMultiByte(code_page, 0, wide.data(), wide_length,
			&mb[0], charcount, NULL, NULL);

		return mb;
	}

	scoped_refptr<UIString> UIString::str()
	{
		return new UIString;
	}

	scoped_refptr<UIString> UIString::str(const wchar_t* str)
	{
		scoped_refptr<UIString> ret(new UIString);
		ret->init(str);
		return ret;
	}

	scoped_refptr<UIString> UIString::str(const char* str)
	{
		scoped_refptr<UIString> ret(new UIString);
		ret->init(str);
		return ret;
	}

	scoped_refptr<UIString> UIString::strWithFormat(const char* format, ...)
	{
		scoped_refptr<UIString> ret(new UIString);
		va_list ap;
		va_start(ap, format);
		ret->initWithFormatV(format, ap);
		va_end(ap);
		
		return ret;
	}

	scoped_refptr<UIString> UIString::strWithFormat(const wchar_t* format, ...)
	{
		scoped_refptr<UIString> ret(new UIString);
		va_list ap;
		va_start(ap, format);
		ret->initWithFormatV(format, ap);
		va_end(ap);
		return ret;
	}


	scoped_refptr<UIString> UIString::strWithString(UIString* str)
	{
		scoped_refptr<UIString> ret(new UIString);
		ret->init(str->utf16_);
		return ret;
	}

	UIString::UIString()
	{

	}

	UIString::~UIString()
	{

	}

	void UIString::init(const std::wstring& s)
	{
		utf16_ = s;
	}

	void UIString::init(const wchar_t* str)
	{
		if (str == NULL) {
			assert(0);
			return;
		}
		utf16_ = str;
	}

	void UIString::init(const char* str)
	{
		if (str == NULL) {
			assert(0);
			return;
		}
		utf16_ = MultiByteToWide(str);
	}

	void UIString::initWithFormat(const char* format, ...)
	{
		va_list ap;
		va_start(ap, format);
		initWithFormatV(format, ap);
		va_end(ap);
	}

	void UIString::initWithFormat(const wchar_t* format, ...)
	{
		va_list ap;
		va_start(ap, format);
		initWithFormatV(format, ap);
		va_end(ap);
	}

	void UIString::initWithFormatV(const char* format, va_list ap)
	{
		std::string utf8;
		StringAppendV(&utf8, format, ap);
		utf16_ = MultiByteToWide(utf8);
	}

	void UIString::initWithFormatV(const wchar_t* format, va_list ap)
	{
		utf16_.clear();
		StringAppendV(&utf16_, format, ap);
	}

	void UIString::initWithString(UIString* str)
	{
		utf16_ = str->utf16_;
	}

}