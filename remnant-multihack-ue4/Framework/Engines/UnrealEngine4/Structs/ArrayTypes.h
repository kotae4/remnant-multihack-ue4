#pragma once

#define _SILENCE_ALL_CXX17_DEPRECATION_WARNING 1
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING 1

#include <string>
#include <codecvt>
#include <locale>
#include <cwchar>

class FString;

// both of these classes taken from knacker's SDK generator (which is taken and stripped from UE)
template<class T>
class TArray
{
	friend class FString;

public:
	TArray()
	{
		Data = nullptr;
		Count = Max = 0;
	};

	size_t Num() const
	{
		return Count;
	};

	T& operator[](size_t i)
	{
		return Data[i];
	};

	const T& operator[](size_t i) const
	{
		return Data[i];
	};

	bool IsValidIndex(size_t i) const
	{
		return i < Num();
	}

private:
	T* Data;
	__int32 Count;
	__int32 Max;
};


class FString : public TArray<wchar_t>
{
public:
	inline FString()
	{
	};

	FString(const wchar_t* other)
	{
		Max = Count = *other ? static_cast<__int32>(std::wcslen(other)) + 1 : 0;

		if (Count)
		{
			Data = const_cast<wchar_t*>(other);
		}
	};

	FString(std::string& other)
	{
		//setup converter
		typedef std::codecvt_utf8<wchar_t> convert_type;
		std::wstring_convert<convert_type, wchar_t> converter;
		//use converter (.to_bytes: wstr->str, .from_bytes: str->wstr)
		std::wstring wOther = converter.from_bytes(other);
		const wchar_t* wOtherArr = wOther.c_str();

		Max = Count = *wOtherArr ? static_cast<__int32>(std::wcslen(wOtherArr)) + 1 : 0;

		if (Count)
		{
			Data = const_cast<wchar_t*>(wOtherArr);
		}
	}

	inline bool IsValid() const
	{
		return Data != nullptr;
	}

	inline const wchar_t* c_str() const
	{
		return Data;
	}

	std::string ToString() const
	{
		const auto length = std::wcslen(Data);

		std::string str(length, '\0');

		std::use_facet<std::ctype<wchar_t>>(std::locale()).narrow(Data, Data + length, '?', &str[0]);

		return str;
	}

	std::wstring ToWString() const
	{
		return std::wstring(Data);
	}
};