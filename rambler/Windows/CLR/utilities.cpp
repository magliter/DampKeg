//2015-01-28

#include "Stdafx.h"

#include "utilities.h"

#include <codecvt>
#include <locale>
#include <msclr/marshal_cppstd.h>

namespace Rambler {

	rambler::String ramblerString(System::String^ dotNetString) {
		static std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		
		std::wstring temp = msclr::interop::marshal_as<std::wstring>(dotNetString);
		return converter.to_bytes(temp);
	}

	System::String^ dotNetString(rambler::String ramblerString) {
		static std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

		std::wstring temp = converter.from_bytes(ramblerString);

		return msclr::interop::marshal_as<System::String ^>(temp);
	}

}