#pragma once

#include "stdafx.h"

#include "CommunicationProtocol.h"
#include "UserData.h"
#include "GameRoom.h"

#define SERVER_PORT 9000
#define BUF_SIZE 100

// For ExternalIP
#define EXTERNALIP_FINDER_URL "http://checkip.dyndns.org/"
#define TITLE_PARSER "<body>Current IP Address: "

// For Login
#define MAX_ID_LEN 12

struct SOCKETINFO
{
	OVERLAPPED overlapped;	// OVERLAPPED 구조체
	WSABUF wsabuf;
	SOCKET sock;
	char buf[BUF_SIZE + 1];
	int dataSize;
	bool isRecvTrue; // 이게 트루면 받을 타이밍, flase면 주는 타이밍
	int bufferProtocol; // 이거는 이제 이 번에 뭘해야하는지 저장해놓는거야 오 좋은데...

	int userIndex;	// 회원가입 + 로그인 시, 로드 유저 데이터에서 해당 인덱스 저장하여 사용!
	
	int roomIndex;	// 방 만들거나 접속 시, 사용될 roomData의 인덱스
	bool isHost; // 인게임에서 호스트역활인지 아닌지 체크.

	BaseStruct* dataBuffer;	// 대망의 이 친구..는 사실 바이트단위로 전송하면서..쓰레기가 되어버렸는데..
};

void err_quit(char *msg) 
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0,
		NULL
	);

	MessageBox(NULL, (LPTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
};

void err_display(char *msg) 
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0,
		NULL
	);
	printf(" [%s]  %S", msg, (char *)lpMsgBuf);
	LocalFree(lpMsgBuf);
};

/*
IOCompletionQueue
FIFO Queue.입출력이 완료된 작업들이 들어간다. thread들은 이 queue에서 작업을 꺼내서 수행한다.

WaitingThreadQueue
LIFO Queue.(왜 이름이 queue지) 작업 대기중인 thread들이 들어있다. 만약 IO가 완료되었다면 이 queue에서 thread를 하나 꺼내서 사용한다.

ReleaseThreadList
현재 작업을 수행하고 있는 thread들이 들어간다.
만약 실행중인 thread가 정지해야 한다면 해당 thread를 PauseThreadList로 보내고 WaitingThreadQueue에서 새로운 thread를 하나 꺼내와서 사용한다. 이 때문에 프로세서의 수보다 많은 thread를 미리 만들어 놓는 것.

PauseThreadList
어떤 원인(임계 구역 등)으로 인해 일시정지된 thread들이 들어간다.
만약 일시정지가 풀리더라도 ReleaseThreadList가 꽉 차있다면 바로 ReleaseThreadList로 보내지 않고 대기한다.
*/

int GetExternalIP(char *ip)
{
	HINTERNET hInternet, hFile;
	DWORD rSize;
	char buffer[256] = { 0 };

	hInternet = InternetOpen(NULL, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);

	if (NULL == hInternet)
		return 0;

	hFile = InternetOpenUrl(hInternet, EXTERNALIP_FINDER_URL, NULL, 0, INTERNET_FLAG_RELOAD, 0);

	if (hFile)
	{
		InternetReadFile(hFile, &buffer, sizeof(buffer), &rSize);
		buffer[rSize] = '\0';

		int nShift = _tcslen(TITLE_PARSER);
		std::string strHTML = buffer;
		std::string::size_type nIdx = strHTML.find(TITLE_PARSER);
		strHTML.erase(strHTML.begin(), strHTML.begin() + nIdx + nShift);
		nIdx = strHTML.find("</body>");
		strHTML.erase(strHTML.begin() + nIdx, strHTML.end());

		_tcscpy(ip, strHTML.c_str());
		InternetCloseHandle(hFile);
		InternetCloseHandle(hInternet);

		return _tcslen(ip);
	}

	return 0;
}


