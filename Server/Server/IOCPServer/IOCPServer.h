#pragma once

#include "../PCH/stdafx.h"

#include "../Protocol/CommunicationProtocol.h"
#include "../IOCPServer/SocketInfo.h"

#include "../IOCPServer/UDPManager.h"

#include "../UserData/UserDataManager.h"
#include "../GameRoom/GameRoomManager.h"

#include "../SceneNetworkManager/BaseScene.h"

#include "../SceneNetworkManager/_0_TitleScene/TitleScene.h"
#include "../SceneNetworkManager/_1_LoginScene/LoginScene.h"
#include "../SceneNetworkManager/_2_MainUIScene/MainUIScene.h"
#include "../SceneNetworkManager/_3_LobbyScene/LobbyScene.h"
#include "../SceneNetworkManager/_4_RoomScene/RoomScene.h"
#include "../SceneNetworkManager/_5_InGameScene/InGameScene.h"

//#define		SERVER_PORT		9000
//#define		SERVER_UDP_PORT 9001

// For ExternalIP
#define		EXTERNALIP_FINDER_URL	"http://checkip.dyndns.org/"
#define		TITLE_PARSER			"<body>Current IP Address: "


//#define SERVER_DEBUG_LOG_PRINT
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
	WSADATA wsa;
	HANDLE hIOCP;
	SOCKET listenSocket;

	SOCKADDR_IN serverAddr;

	HANDLE hManagerThread;
	HANDLE hUDPThread;

	//For Game
	UserDataManager userData;
	GameRoomManager roomData;

	UDPManager udpManager;

	SCENE_NETWORK_MANAGER::BaseScene* sceneNetworkManagerArr[6];
	//void(*SceneDataProcess[6])(const int& InRecvType,SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);

public:
	//IOCPServer() : bIsPrintDebugMessage(false)
	//{
	//	Init();
	//}

	IOCPServer(bool InIsTrueLoadExternIP) 
	{
			Init(InIsTrueLoadExternIP);
	}

	~IOCPServer()
	{
		Close();
	}
	
public:
	// Init Server
	void Init(bool InIsTrueLoadExternIP)
	{
		_PrintServerInfoUI(InIsTrueLoadExternIP);
		//LoadUserData();
		_InitWinSocket();
		_CreateBindListen();
		_BindSceneDataProcess();

		_CreateSocket();
	}

	void Run()
	{
		_RunOtherThread();
		_AcceptProcess();
	}

	void Close()
	{
		_DestroyAndClean();
	}

private:
	//Init
	int _GetExternalIP(char *ip);

	void _PrintServerInfoUI(const bool& InIsTrueLoadExternIP = true);
	
	//void LoadUserData();
	
	void _InitWinSocket();

	void _CreateBindListen();

	void _BindSceneDataProcess();

	//Init - For UDP [DEV_53]
	void _CreateSocket();

	//Run
	void _AcceptProcess();

	void _RunOtherThread();

	//Close
	void _DestroyAndClean();

private:
	//ThreadFunction
	static DWORD WINAPI WorkerThread(LPVOID arg);
	void _WorkerThreadFunction();

	static DWORD WINAPI UDPThread(LPVOID arg);
	void _UDPThreadFunction();

	static DWORD WINAPI ManagerThread(LPVOID arg);
	void _ManagerLoop();
};
