#pragma once

#include <Windows.h>
#include <string>
#include <vector>

namespace Utils
{
	namespace Strings
	{
		// credit: someone on stackoverflow, probably (sorry didn't bookmark)
		void PrintErrorMessage(HRESULT error);
		std::string GetErrorMessage(HRESULT error);
		VOID PrintLastError();

		// credit: https://docs.microsoft.com/en-us/archive/msdn-magazine/2016/september/c-unicode-encoding-conversions-with-stl-strings-and-win32-apis
		BOOL TryConvertUtf8ToUtf16(const std::string& utf8, OUT std::wstring& outWStr);
		BOOL TryConvertUtf16ToUtf8(const std::wstring& utf16, OUT std::string& outStr);

		// credit: https://stackoverflow.com/a/57346888
		std::vector<std::string> SplitA(const std::string& input, const std::string& delim);
		std::vector<std::wstring> SplitW(const std::wstring& input, const std::wstring& delim);
	}
};

