#include "../PCH/stdafx.h"

_NODISCARD string CONVERT_UTIL::WStringToString(wstring& InWString)
{
	int sizeBuffer = WideCharToMultiByte(CP_ACP, 0, &InWString[0], -1, NULL, 0, NULL, NULL);
	
	string retString(sizeBuffer, 0);

	WideCharToMultiByte(CP_ACP, 0, &InWString[0], -1, &retString[0], sizeBuffer, NULL, NULL);

	return retString;
}