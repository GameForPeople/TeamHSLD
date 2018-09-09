#pragma once

#include "../stdafx.h"

#include "../Protocol/CommunicationProtocol.h"
#include "../UserData/UserData.h"
#include "../GameRoom/GameRoom.h"


#define		SERVER_PORT		9000
#define		BUF_SIZE		100

// For ExternalIP
#define		EXTERNALIP_FINDER_URL	"http://checkip.dyndns.org/"
#define		TITLE_PARSER			"<body>Current IP Address: "

// For Login
#define		MAX_ID_LEN		12

//#define SERVER_DEBUG_LOG_PRINT

struct SOCKETINFO
{
	OVERLAPPED overlapped;	// OVERLAPPED ����ü
	WSABUF wsabuf;
	SOCKET sock;
	char buf[BUF_SIZE + 1];
	int dataSize;
	
	// �̰� Ʈ��� ���� Ÿ�̹�, flase�� �ִ� Ÿ�̹� (�ߺ�! ���� �踤�Դϴ�.)
	bool isRecvTrue; 
	
	// �̰Ŵ� �� ���Ͽ� ������ �ؾ������� �����س��� �� (enum Protocol)
	int bufferProtocol; 

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
	BaseStruct* dataBuffer;	
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

// IOCP Ŭ�������� Ż��... 
//�� ģ���� ������ ����ؼ� ���� �̱�����...?
// ������ ���� ��, ������ ���ڷ� �ѱ�°� ������..
//CUserData userData;
//CGameRoom roomData;
// �ϴ� ���� ���������, �� �����忡�� �ѱ⵵���սô�;;

class IOCPServer {
private:
	bool bIsPrintDebugMessage;

	WSADATA wsa;
	HANDLE hIOCP;
	SOCKET listenSocket;
	SOCKADDR_IN serverAddr;

	HANDLE hSaveUserDataThread;
	bool isOnSaveUserDataThread;

	//For Game
	CUserData userData;
	CGameRoom roomData;

	//std::atomic_bool isSaveOn; // ���� ���� ���� �ʿ����.. ����ȭ ���� �Ͽ� ��¥�� ��.
	bool isSaveOn;
	int selfControl;

public:
	__inline IOCPServer() : bIsPrintDebugMessage(false)
	{
		Init();
	}

	__inline IOCPServer(bool InIsPrintDebugMessage) 
		: bIsPrintDebugMessage(InIsPrintDebugMessage)
	{
		Init();
	}

	__inline ~IOCPServer()
	{
		Close();
	}
	
public:
	// Init Server
	void Init()
	{
		PrintServerInfoUI();
		LoadUserData();
		InitWinSocket();
		CreateBindListen();
	}

	void Run()
	{
		AcceptProcess();
	}

	void RunSaveUserDataThread() // ���Ŀ�, Run�� ���� �̴ϴ�.
	{
		hSaveUserDataThread = CreateThread(NULL, 0, SaveUserDate, (LPVOID)this, 0, NULL);
		//CloseHandle(hSaveUserDataThread);
	}

	void Close()
	{
		DestroyAndClean();
	}

private:
	//Init
	int GetExternalIP(char *ip);

	void PrintServerInfoUI();
	
	void LoadUserData();
	
	void InitWinSocket();

	void CreateBindListen();

	//Run
	void AcceptProcess();

	//Close
	void DestroyAndClean();

	//ThreadFunction
	static DWORD WINAPI WorkerThread(LPVOID arg);
	void WorkerThreadFunction();
	static DWORD WINAPI SaveUserDate(LPVOID arg);
	void SaveUserDataThreadFunction();

private:

};
