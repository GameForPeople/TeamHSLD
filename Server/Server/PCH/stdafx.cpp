#include "../PCH/stdafx.h"

_NODISCARD string CONVERT_UTIL::WStringToString(wstring& InWString)
{
	int sizeBuffer = WideCharToMultiByte(CP_ACP, 0, &InWString[0], -1, NULL, 0, NULL, NULL);
	
	string retString(sizeBuffer, 0);

	WideCharToMultiByte(CP_ACP, 0, &InWString[0], -1, &retString[0], sizeBuffer, NULL, NULL);

	// FixError ==
	retString.pop_back(); //(retString.end(), retString.end());
	//retString.insert(retString.end(), '\0');

	return retString;
}

_NODISCARD wstring CONVERT_UTIL::StringToWString(string& InString)
{
	int sizeBuffer = MultiByteToWideChar(CP_ACP, 0, &InString[0], -1, NULL, 0);

	wstring retString(sizeBuffer, 0);

	MultiByteToWideChar(CP_ACP, 0, &InString[0], -1, &retString[0], sizeBuffer);

	return retString;
}