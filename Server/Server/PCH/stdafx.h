#pragma once

// -----------------------------------
#define _DEBUG_MODE_
// -----------------------------------
// �ش� Define�� ���߿� �α׸� ����ϱ� ���� ����Ǹ�, ������ �ÿ��� �� Off�� �ʿ��մϴ�.
// �׷���, ������ ġ������ ������ �߻��� ��, ��µǴ� �α״� �׻� ��µ˴ϴ�.
/*
	Define���ο� ������� ��µǴ� Log ���
		0. Server Core UI ( IP, Thread State )
		1. Server Fatal Error
		2. Managed Thread's Output
		3. �����ϱ� ���� ��Ȳ�� ���� �α� ( ģ�� �г��� ����, UDP - shared_ptr ���� ��)

	_DEBUG_MODE_ �� Log ��� ()
		0. Login, LogOut User 
		1. Send or Recv Data Protocol
		2. ���� ���� ��, �� ���� ���
		3. ���� ���� ��, ��� ��� (���� ������)

	��Ÿ ���� �̽� ������ ���� ������ Log�� ���� Ȥ�� �ּ� ó��(�ʿ� ��, �׽�Ʈ)�� ��Ģ���� �մϴ�.
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
#define		VIP_CODE_STRING			L"¥����� ¥��¥��"	// �ش� ��Ʈ���� �� wstring�̿��� �մϴ�.

using namespace std;

using Type_Nickname = std::basic_string<char, char_traits<char>, allocator<char>>;	/* == string */
using Type_ID = std::basic_string<char, char_traits<char>, allocator<char>>;		/* == string */

//using tstring = std::basic_string<TCHAR>; //��Ƽ����Ʈ�� �����ϰ�, �г��Ӹ� TCHAR �����ϴ°ɷ� ����.

namespace CONVERT_UTIL
{
	_NODISCARD string WStringToString(wstring& InWstring);
	_NODISCARD wstring StringToWString(string& InString);
}
