#pragma once

#include <string>

namespace ui
{
	/*
	*      ftp://username:password@hostname:portnumber/pathname#ref
	*      \ /   \               / \      / \        /\         \ /
	*       -     ---------------   ------   --------  |         -
	*       |            |             |        |      |         |
	*       |            |             |        |      |        Ref
	*       |            |             |       Port    \        /
	*       |            |            Host      /       --------
	*       |         UserPass                 /	         |
	*     Scheme                              /	        Path
	*       \                                /
	*        --------------------------------
	*                       |
	*                    PrePath
	*/
	class URLRef;
	class URL
	{
	public:
		// Creates an empty, invalid URL.
		URL();

		URL(const URL& other);
		~URL();

		explicit URL(const std::string& url_string);
		explicit URL(const std::wstring& url_string);

		URL& operator=(const URL& other);

		void Reset(URLRef* url);
		bool IsEquals(const URL& other) const;

		bool is_valid() const;
		bool is_empty() const;

		const std::string& spec() const;

		URL Resolve(const std::string& relative) const;
		URL Resolve(const std::wstring& relative) const;
		URL GetWithEmptyPath() const;
		URL GetOrigin() const;

		std::string scheme() const;
		std::string username() const;
		std::string password() const;
		std::string host() const;
		std::string port() const;
		std::string path() const;
		std::string query() const;
		std::string ref() const;

		bool has_scheme() const;
		bool has_username() const;
		bool has_password() const;
		bool has_host() const;
		bool has_port() const;
		bool has_path() const;
		bool has_query() const;
		bool has_ref() const;

		bool SchemeIs(const char* lower_ascii_scheme) const;
	private:
		URLRef* url_;
	};
}