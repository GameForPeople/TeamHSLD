#pragma once

#pragma comment(lib, "ws2_32")
#pragma comment(lib, "wininet.lib")

#include <WinSock2.h>
#include <iostream>
#include <cstdlib>

#include <fstream>

#include <vector>
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
#define		MAX_ID_LEN		12
#define		BUF_SIZE		100

struct SOCKETINFO
{
	OVERLAPPED overlapped;	// OVERLAPPED ����ü
	WSABUF wsabuf;
	SOCKET sock;
	char buf[BUF_SIZE + 1];
	
	int dataSize; 

	// �̰� Ʈ��� ���� Ÿ�̹�, flase�� �ִ� Ÿ�̹� (�ߺ�! ���� �踤�Դϴ�.)
	bool isRecvTurn;

	// �̰Ŵ� �� ���Ͽ� ������ �ؾ������� �����س��� �� (enum Protocol)
	//int bufferProtocol; // �Ƚ��Ӹ�

	// ȸ������ + �α��� ��, �ε� ���� �����Ϳ��� �ش� �ε��� �����Ͽ� ���!	
	// ���Ŀ� �ش� �ε����� ���ͷ����� �����ͷ� ���� ������ �����Դϴ�.
	// ���� �����̳� Std::Vector
	int userIndex;

	// �뾻 �ε���, �� ���� �ó� ���� �� ���� ( �ʱ�ȭ ���ʿ� )
	int roomIndex;

	// �뾻, �ΰ��Ӿ����� ȣ��Ʈ ���� üũ (�ʱ�ȭ ���ʿ�)
	bool isHost;

	// ����� �� ģ��..�� ��� ����Ʈ������ �����ϸ鼭..�����Ⱑ �Ǿ���ȴµ�..
	//�� �ٽ� ��Ȱ!!!! �����丵 �������� �̰� ����ϱ�� ����!!! �ּ� ����!! ������ �Ф�
	// �� memcpy ���������� ����������... �Ⱦ��°ɷ� ����..
	//BaseStruct* dataBuffer;

public:
	//������ �ʿ信 ���� �߰�.
	__inline SOCKETINFO() :
		//bufferProtocol(0), 
		buf(),
		userIndex(-1), 
		roomIndex(-1), 
		isHost(false),
		//dataSize(), 
		isRecvTurn(true)
	{};

	__inline ~SOCKETINFO() = default;
};
