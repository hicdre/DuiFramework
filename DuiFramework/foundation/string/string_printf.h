#pragma once

#include <stdarg.h>   // va_list
#include <string>


// Return a C++ string given printf-like input.
std::string StringPrintf(const char* format, ...);
std::wstring StringPrintf(const wchar_t* format, ...);

// Return a C++ string given vprintf-like input.
std::string StringPrintV(const char* format, va_list ap);

// Store result into a supplied string and return it.
const std::string& SStringPrintf(std::string* dst, const char* format, ...);
const std::wstring& SStringPrintf(std::wstring* dst, const wchar_t* format, ...);

// Append result to a supplied string.
void StringAppendF(std::string* dst, const char* format, ...);
void StringAppendF(std::wstring* dst, const wchar_t* format, ...);

// Lower-level routine that takes a va_list and appends to a specified
// string.  All other routines are just convenience wrappers around it.
void StringAppendV(std::string* dst, const char* format, va_list ap);
void StringAppendV(std::wstring* dst, const wchar_t* format, va_list ap);


