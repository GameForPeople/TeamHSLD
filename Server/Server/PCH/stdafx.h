#pragma once

#pragma comment(lib, "ws2_32")
#pragma comment(lib, "wininet.lib")

#include <WinSock2.h>

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <utility>

//
#include <fstream>
#include <string>

//STL
#include <vector>
#include <map>
#include <queue>

//thread
#include <thread>
#include <atomic>
#include <mutex>

// Function Pointer
#include <functional>

// For ExternalIP
#include "wininet.h"
#include <tchar.h>

// C++17
#include <string_view>

// DEV_66
#include <clocale>
#include <codecvt>
#include <mbstring.h>
#include <Mbctype.h>
#include <AtlBase.h>
#include <atlconv.h>

// custom

// For Login
#define		MAX_ID_LEN			30
#define		BUF_SIZE			100

#define		SERVER_PORT			9000
#define		SERVER_UDP_PORT		9001

#define		_NORETURN			[[noreturn]]
#define		_NODISCARD			[[nodiscard]]
#define		_DEPRECATED			[[deprecated]]
#define		_MAYBE_UNUSED		[[maybe_unused]]
#define		_FALLTHROUGH		[[fallthrough]]

#define		_TEST_FUNCTION		[[deprecated]]

// For ExternalIP
#define		EXTERNALIP_FINDER_URL	TEXT("http://checkip.dyndns.org/")
#define		TITLE_PARSER			TEXT("<body>Current IP Address: ")

// FOR VIP CODE
#define		VIP_CODE_STRING			L"짜장면은 짜장짜장"	// 해당 스트링은 꼭 wstring이여야 합니다.

using namespace std;

using Type_Nickname = std::basic_string<char, char_traits<char>, allocator<char>>;	/* == string */
using Type_ID = std::basic_string<char, char_traits<char>, allocator<char>>;		/* == string */

//using tstring = std::basic_string<TCHAR>; //멀티바이트로 정의하고, 닉네임만 TCHAR 적용하는걸로 변경.

namespace CONVERT_UTIL
{
	_NODISCARD string WStringToString(wstring& InWstring);
}
