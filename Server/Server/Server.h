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
	OVERLAPPED overlapped;	// OVERLAPPED ����ü
	WSABUF wsabuf;
	SOCKET sock;
	char buf[BUF_SIZE + 1];
	int dataSize;
	bool isRecvTrue; // �̰� Ʈ��� ���� Ÿ�̹�, flase�� �ִ� Ÿ�̹�
	int bufferProtocol; // �̰Ŵ� ���� �� ���� ���ؾ��ϴ��� �����س��°ž� �� ������...

	int userIndex;	// ȸ������ + �α��� ��, �ε� ���� �����Ϳ��� �ش� �ε��� �����Ͽ� ���!
	
	int roomIndex;	// �� ����ų� ���� ��, ���� roomData�� �ε���
	bool isHost; // �ΰ��ӿ��� ȣ��Ʈ��Ȱ���� �ƴ��� üũ.

	BaseStruct* dataBuffer;	// ����� �� ģ��..�� ��� ����Ʈ������ �����ϸ鼭..�����Ⱑ �Ǿ���ȴµ�..
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
FIFO Queue.������� �Ϸ�� �۾����� ����. thread���� �� queue���� �۾��� ������ �����Ѵ�.

WaitingThreadQueue
LIFO Queue.(�� �̸��� queue��) �۾� ������� thread���� ����ִ�. ���� IO�� �Ϸ�Ǿ��ٸ� �� queue���� thread�� �ϳ� ������ ����Ѵ�.

ReleaseThreadList
���� �۾��� �����ϰ� �ִ� thread���� ����.
���� �������� thread�� �����ؾ� �Ѵٸ� �ش� thread�� PauseThreadList�� ������ WaitingThreadQueue���� ���ο� thread�� �ϳ� �����ͼ� ����Ѵ�. �� ������ ���μ����� ������ ���� thread�� �̸� ����� ���� ��.

PauseThreadList
� ����(�Ӱ� ���� ��)���� ���� �Ͻ������� thread���� ����.
���� �Ͻ������� Ǯ������ ReleaseThreadList�� �� ���ִٸ� �ٷ� ReleaseThreadList�� ������ �ʰ� ����Ѵ�.
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


