#pragma once

#include "../PCH/stdafx.h"

#include "../Protocol/CommunicationProtocol.h"
#include "../IOCPServer/SocketInfo.h"

#include "../UserData/UserDataManager.h"
#include "../GameRoom/GameRoomManager.h"

#include "../SceneNetworkManager/BaseScene.h"

#include "../SceneNetworkManager/_0_TitleScene/TitleScene.h"
#include "../SceneNetworkManager/_1_LoginScene/LoginScene.h"
#include "../SceneNetworkManager/_2_MainUIScene/MainUIScene.h"
#include "../SceneNetworkManager/_3_LobbyScene/LobbyScene.h"
#include "../SceneNetworkManager/_4_RoomScene/RoomScene.h"
#include "../SceneNetworkManager/_5_InGameScene/InGameScene.h"

#define		SERVER_PORT		9000
#define		SERVER_UDP_PORT 9001

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
	bool bIsPrintDebugMessage;

	WSADATA wsa;
	HANDLE hIOCP;
	SOCKET listenSocket;
	SOCKET udpSocket;

	SOCKADDR_IN serverAddr;
	SOCKADDR_IN serverUDPAddr;

	HANDLE hManagerThread;

	//For Game
	UserDataManager userData;
	GameRoomManager roomData;

	SCENE_NETWORK_MANAGER::BaseScene* sceneNetworkManagerArr[6];
	//void(*SceneDataProcess[6])(const int& InRecvType,SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);

	//std::atomic_bool isSaveOn; // ���� ���� ���� �ʿ����..���� �Ͽ� ��¥�� ����ȭ ��.
	bool isSaveOn;

	bool isSendUDPMessage;
	string udpMessageBuffer;

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
		_PrintServerInfoUI();
		//LoadUserData();
		_InitWinSocket();
		_CreateBindListen();
		_BindSceneDataProcess();

		_CreateUDPSocket();
	}

	void Run()
	{
		_RunSaveUserDataThread();
		_AcceptProcess();
	}

	void Close()
	{
		_DestroyAndClean();
	}

private:
	//Init
	int _GetExternalIP(char *ip);

	void _PrintServerInfoUI();
	
	//void LoadUserData();
	
	void _InitWinSocket();

	void _CreateBindListen();

	void _BindSceneDataProcess();

	//Init - For UDP [DEV_53]
	void _CreateUDPSocket();

	//Run
	void _AcceptProcess();

	void _RunSaveUserDataThread()
	{
		hManagerThread = CreateThread(NULL, 0, ManagerThread, (LPVOID)this, 0, NULL);
		//CloseHandle(hSaveUserDataThread);
		std::cout << "     [UserDataManager] Run Save Thread! " << "\n";
	}
	//Close
	void _DestroyAndClean();

private:
	//ThreadFunction
	static DWORD WINAPI WorkerThread(LPVOID arg);
	void _WorkerThreadFunction();
	static DWORD WINAPI ManagerThread(LPVOID arg);
	void _ManagerLoop();
	// void SaveUserData();

	// UDPSocket
	void _SendDynamicMessage();
};
