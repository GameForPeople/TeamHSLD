#pragma once

#include "../PCH/stdafx.h"

#include "../Protocol/CommunicationProtocol.h"
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
	SOCKADDR_IN serverAddr;

	HANDLE hSaveUserDataThread;
	bool isOnSaveUserDataThread;

	//For Game
	UserDataManager userData;
	GameRoomManager roomData;

	//Scene //���� BaseScene �����ͷ� ����, ������ �ʿ� ����.-> �� �ȵǴ� ���� Ȯ��...
	//SCENE_NETWORK_MANAGER::TitleScene titleScene;
	//SCENE_NETWORK_MANAGER::LoginScene loginScene;
	//SCENE_NETWORK_MANAGER::MainUiScene mainUiScene;
	//SCENE_NETWORK_MANAGER::LobbyScene lobbyScene;
	//SCENE_NETWORK_MANAGER::RoomScene roomScene;
	//SCENE_NETWORK_MANAGER::InGameScene inGameScene;

	//std::vector<SCENE_NETWORK_MANAGER::BaseScene*> sceneArr;
	SCENE_NETWORK_MANAGER::BaseScene* sceneArr[6];

	//void(*SceneDataProcess[6])(const int& InRecvType,SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);

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
		BindSceneDataProcess();
	}

	void Run()
	{
		RunSaveUserDataThread();
		AcceptProcess();
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

	void BindSceneDataProcess();

	//Run
	void AcceptProcess();

	void RunSaveUserDataThread()
	{
		hSaveUserDataThread = CreateThread(NULL, 0, SaveUserDate, (LPVOID)this, 0, NULL);
		//CloseHandle(hSaveUserDataThread);
		std::cout << "     [UserDataManager] Run Save Thread! " << "\n";
	}
	//Close
	void DestroyAndClean();

	//ThreadFunction
	static DWORD WINAPI WorkerThread(LPVOID arg);
	void WorkerThreadFunction();
	static DWORD WINAPI SaveUserDate(LPVOID arg);
	void SaveUserDataThreadFunction();

private:

};
