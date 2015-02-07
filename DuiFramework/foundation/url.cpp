#include "stdafx.h"
#include "url.h"

#include "foundation/basictypes.h"
#include "foundation/base/ref_counted.h"
#include "foundation/string/icu_utf.h"
#include "third_party/gurl/gurl.h"
#include "third_party/gurl/url_canon_internal.h"


namespace url_canon {
	bool ReadUTFChar(const char* str, int* begin, int length,
		unsigned* code_point_out) {
		int code_point;  // Avoids warning when U8_NEXT writes -1 to it.
		CBU8_NEXT(str, *begin, length, code_point);
		*code_point_out = static_cast<unsigned>(code_point);

		// The ICU macro above moves to the next char, we want to point to the last
		// char consumed.
		(*begin)--;

		// Validate the decoded value.
		if (CBU_IS_UNICODE_CHAR(code_point))
			return true;
		*code_point_out = kUnicodeReplacementCharacter;
		return false;
	}

	bool ReadUTFChar(const wchar_t* str, int* begin, int length,
		unsigned* code_point) {
		if (CBU16_IS_SURROGATE(str[*begin])) {
			if (!CBU16_IS_SURROGATE_LEAD(str[*begin]) || *begin + 1 >= length ||
				!CBU16_IS_TRAIL(str[*begin + 1])) {
				// Invalid surrogate pair.
				*code_point = kUnicodeReplacementCharacter;
				return false;
			}
			else {
				// Valid surrogate pair.
				*code_point = CBU16_GET_SUPPLEMENTARY(str[*begin], str[*begin + 1]);
				(*begin)++;
			}
		}
		else {
			// Not a surrogate, just one 16-bit word.
			*code_point = str[*begin];
		}

		if (CBU_IS_UNICODE_CHAR(*code_point))
			return true;

		// Invalid code point.
		*code_point = kUnicodeReplacementCharacter;
		return false;
	}
}

namespace ui
{
	class URLRef : public GURL, public RefCounted < URLRef >
	{
	public:
		URLRef() : GURL() {}
		explicit URLRef(const std::string& url_string) : GURL(url_string) {}
		explicit URLRef(const std::wstring& url_string) : GURL(url_string) {}
		URLRef(const GURL& url) : GURL(url) {}

		static URLRef* empty_url_;
		static URLRef* CreateEmpty()
		{
			if (!empty_url_)
			{
				empty_url_ = new URLRef;
				empty_url_->AddRef();
			}
			return empty_url_;
		}

	private:
		DISALLOW_COPY_AND_ASSIGN(URLRef);
	};

	URLRef* URLRef::empty_url_ = NULL;
	

	URL::URL()
		: url_(URLRef::CreateEmpty())
	{
		url_->AddRef();
	}

	URL::URL(const URL& other)
		: url_(other.url_)
	{
		if (url_)
			url_->AddRef();
	}

	URL::URL(const std::string& url_string)
		: url_(new URLRef(url_string))
	{
		url_->AddRef();
	}

	URL::URL(const std::wstring& url_string)
		: url_(new URLRef(url_string))
	{
		url_->AddRef();
	}

	URL::~URL()
	{
		url_->Release();
	}

	void URL::Reset(URLRef* url)
	{
		url_->Release();
		url_ = url;
		url_->AddRef();
	}

	bool URL::is_valid() const
	{
		return url_->is_valid();
	}

	bool URL::is_empty() const
	{
		return url_->is_empty();
	}

	const std::string& URL::spec() const
	{
		return url_->spec();
	}

	ui::URL URL::Resolve(const std::string& relative) const
	{
		URL url;
		url.Reset(new URLRef(url_->Resolve(relative)));
		return url;
	}

	ui::URL URL::Resolve(const std::wstring& relative) const
	{
		URL url;
		url.Reset(new URLRef(url_->Resolve(relative)));
		return url;
	}

	ui::URL URL::GetWithEmptyPath() const
	{
		URL url;
		url.Reset(new URLRef(url_->GetWithEmptyPath()));
		return url;
	}

	ui::URL URL::GetOrigin() const
	{
		URL url;
		url.Reset(new URLRef(url_->GetOrigin()));
		return url;
	}

	std::string URL::scheme() const
	{
		return url_->scheme();
	}

	std::string URL::username() const
	{
		return url_->username();
	}

	std::string URL::password() const
	{
		return url_->password();
	}

	std::string URL::host() const
	{
		return url_->host();
	}

	std::string URL::port() const
	{
		return url_->port();
	}

	std::string URL::path() const
	{
		return url_->path();
	}

	std::string URL::query() const
	{
		return url_->query();
	}

	std::string URL::ref() const
	{
		return url_->ref();
	}

	bool URL::has_scheme() const
	{
		return url_->has_scheme();
	}

	bool URL::has_username() const
	{
		return url_->has_username();
	}

	bool URL::has_password() const
	{
		return url_->has_password();
	}

	bool URL::has_host() const
	{
		return url_->has_host();
	}

	bool URL::has_port() const
	{
		return url_->has_port();
	}

	bool URL::has_path() const
	{
		return url_->has_path();
	}

	bool URL::has_query() const
	{
		return url_->has_query();
	}

	bool URL::has_ref() const
	{
		return url_->has_ref();
	}

	bool URL::SchemeIs(const char* lower_ascii_scheme) const
	{
		return url_->SchemeIs(lower_ascii_scheme);
	}

	URL& URL::operator=(const URL& other)
	{
		url_->Release();
		url_ = other.url_;
		url_->AddRef();
		return *this;
	}

	bool URL::IsEquals(const URL& other) const
	{
		if (url_ == other.url_)
			return true;

		return url_->possibly_invalid_spec() == other.url_->possibly_invalid_spec();
	}

}