#pragma once
#include "Base/Definitions.h"
#include "utf8/utf8.h"

namespace utf_converter
{
	FORCEINLINE std::string utf16_to_utf8(std::wstring utf16_string)
	{
        std::string utf8_string;
		utf8::utf16to8(utf16_string.begin(), utf16_string.end(), back_inserter(utf8_string));
        return utf8_string;
	}

	FORCEINLINE std::wstring utf8_to_utf16(std::string utf8_string)
	{
		std::wstring utf16_string;
		utf8::utf8to16(utf8_string.begin(), utf8_string.end(), back_inserter(utf16_string));
		return utf16_string;
	}
}