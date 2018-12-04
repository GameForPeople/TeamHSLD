#pragma once

#pragma comment(lib, "ws2_32")
#pragma comment(lib, "wininet.lib")

#include <WinSock2.h>

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <utility>

#include <fstream>

#include <vector>
#include <map>
#include <queue>
#include <string>

//thread
#include <thread>
#include <atomic>
#include <mutex>

// For ExternalIP
#include "wininet.h"
#include <tchar.h>


// C++17
#include <string_view>

// custom

// For Login
#define		MAX_ID_LEN			30
#define		BUF_SIZE			100

#define		SERVER_PORT			9000
#define		SERVER_UDP_PORT		9001

#define		_NORETURN			[[noreturn]]
#define		_NODISCARD			[[nodiscard]]

using namespace std;

//using tstring = std::basic_string<TCHAR>; //멀티바이트로 정의하고, 닉네임만 wstring적용하는걸로 변경.

