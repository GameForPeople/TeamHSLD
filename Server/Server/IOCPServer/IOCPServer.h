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
	OVERLAPPED overlapped;	// OVERLAPPED 구조체
	WSABUF wsabuf;
	SOCKET sock;
	char buf[BUF_SIZE + 1];
	int dataSize;
	
	// 이게 트루면 받을 타이밍, flase면 주는 타이밍 (삐빅! 지금 계륵입니다.)
	bool isRecvTrue; 
	
	// 이거는 이 번턴에 무엇을 해야할지를 저장해놓는 것 (enum Protocol)
	int bufferProtocol; 

	// 회원가입 + 로그인 시, 로드 유저 데이터에서 해당 인덱스 저장하여 사용!	
	// 추후에 해당 인덱스는 이터레이터 포인터로 변경 적용할 예정입니다.
	// 현재 컨테이너 Std::Vector
	int userIndex;	
	
	// 룸씐 인덱스, 방 제작 시나 접속 시 사용됨 ( 초기화 미필요 )
	int roomIndex;	
	
	// 룸씐, 인게임씬에서 호스트 여부 체크 (초기화 미필요)
	bool isHost; 

	// 대망의 이 친구..는 사실 바이트단위로 전송하면서..쓰레기가 되어버렸는데..
	//는 다시 부활!!!! 리팩토링 과정에서 이거 사용하기로 변경!!! 주석 해제!! 축하해 ㅠㅠ
	BaseStruct* dataBuffer;	
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

// IOCP 클래스에서 탈출... 
//이 친구들 백프로 써야해서 굳이 싱글톤을...?
// 전역이 좋을 까, 쓰레도 인자로 넘기는게 좋을까..
//CUserData userData;
//CGameRoom roomData;
// 일단 서버 멤버변수로, 각 쓰레드에는 넘기도록합시다;;

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

	//std::atomic_bool isSaveOn; // 굳이 성능 떨굴 필요없음.. 동기화 다음 턴에 어짜피 됨.
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

	void RunSaveUserDataThread() // 추후에, Run에 넣을 겁니다.
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
