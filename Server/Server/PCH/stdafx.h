#pragma once

#pragma comment(lib, "ws2_32")
#pragma comment(lib, "wininet.lib")

#include <WinSock2.h>
#include <iostream>
#include <cstdlib>
#include <ctime>

#include <fstream>

#include <vector>
#include <map>

#include <string>

//thread
#include <thread>
#include <atomic>
#include <mutex>

// For ExternalIP
#include "wininet.h"
#include <tchar.h>

#include <string_view>

using namespace std;

// For Login
#define		MAX_ID_LEN		30
#define		BUF_SIZE		100

