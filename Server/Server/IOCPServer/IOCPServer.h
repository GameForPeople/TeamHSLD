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
	UserDataManager userData;
	GameRoomManager roomData;

	//Scene //굳이 BaseScene 포인터로 저짓, 이짓할 필요 없음.-> 이 안되는 것을 확인...
	//SCENE_NETWORK_MANAGER::TitleScene titleScene;
	//SCENE_NETWORK_MANAGER::LoginScene loginScene;
	//SCENE_NETWORK_MANAGER::MainUiScene mainUiScene;
	//SCENE_NETWORK_MANAGER::LobbyScene lobbyScene;
	//SCENE_NETWORK_MANAGER::RoomScene roomScene;
	//SCENE_NETWORK_MANAGER::InGameScene inGameScene;

	//std::vector<SCENE_NETWORK_MANAGER::BaseScene*> sceneArr;
	SCENE_NETWORK_MANAGER::BaseScene* sceneArr[6];

	//void(*SceneDataProcess[6])(const int& InRecvType,SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);

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
