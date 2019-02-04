#pragma once

// -----------------------------------
#define _DEBUG_MODE_
// -----------------------------------
// 해당 Define은 개발용 로그를 출력하기 위해 선언되며, 릴리즈 시에는 꼭 Off가 필요합니다.
// 그러나, 서버에 치명적인 오류가 발생할 시, 출력되는 로그는 항상 출력됩니다.
/*
	Define여부와 상관없이 출력되는 Log 목록
		0. Server Core UI ( IP, Thread State )
		1. Server Fatal Error
		2. Managed Thread's Output
		3. 존재하기 힘든 상황에 대한 로그 ( 친구 닉네임 삭제, UDP - shared_ptr 의존 등)

	_DEBUG_MODE_ 의 Log 목록 ()
		0. Login, LogOut User 
		1. Send or Recv Data Protocol
		2. 게임 시작 시, 방 정보 출력
		3. 게임 종료 시, 결과 출력 (아직 미적용)

	기타 단위 이슈 개발을 위해 존재한 Log는 삭제 혹은 주석 처리(필요 시, 테스트)를 원칙으로 합니다.
*/

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
// include <AtlBase.h>
// include <atlconv.h>

// for bit
#include <bitset>

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
	_NODISCARD wstring StringToWString(string& InString);
}
