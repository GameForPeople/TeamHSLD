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

class IOCPServer {
public:
	enum class SERVER_INIT_TYPE : int
	{
		EXTERNAL_IP = 1
		, LOCAL_HOST = 2
		, PUBLIC_IP = 3
	};

private:
	WSADATA				wsa;
	HANDLE				hIOCP;
	SOCKET				listenSocket;

	SOCKADDR_IN			serverAddr;

	HANDLE				hManagerThread;
	HANDLE				hUDPSendThread;
	HANDLE				hUDPRecvThread;


	// Only Use 
	UserDataManager*	pUserData;
	GameRoomManager*	pRoomData;
	UDPManager*			pUdpManager;

	std::vector<unique_ptr<SCENE_NETWORK_MANAGER::BaseScene>> sceneNetworkManagerCont;

public:
	IOCPServer(const SERVER_INIT_TYPE InServerInitType)
		: wsa()
		, hIOCP()
		, listenSocket()
		, serverAddr()
		, pUserData(new UserDataManager())
		, pRoomData(new GameRoomManager())
		, pUdpManager(new UDPManager())
		, hManagerThread()
		, hUDPSendThread()
		, hUDPRecvThread()
	{
		Init(InServerInitType);
	}

	~IOCPServer()
	{
		sceneNetworkManagerCont.clear();

		delete pUserData;
		delete pRoomData;
		delete pUdpManager;

		try { Close(); }
		catch (...)
		{
			std::cout << "[서버 오류] : IOCPServer 소멸자에서 예외가 발생했습니다. (사실 상관없어요!)" << "\n";
			std::exit(EXIT_FAILURE);
		}
	}
	
public:
	// Init Server
	void Init(const SERVER_INIT_TYPE InIsTrueLoadExternalIP)
	{
		// 윈도우 서버 OS 대응.
		setlocale(LC_ALL, "Korean");

		_wsetlocale(LC_ALL, L"Korean");

		_PrintServerInfoUI(InIsTrueLoadExternalIP);
		//LoadUserData();	// 나중에 닉네임 해싱 트리 만드는 작업 여기서 필요함.

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
	void _GetExternalIP(char *ip);

	void _PrintServerInfoUI(const SERVER_INIT_TYPE InIsTrueLoadExternalIP);	// 서버 관리자에게 필요한 UI를 띄어줍니다.
	
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

	static DWORD WINAPI UDPSendThread(LPVOID arg);
	void _UDPSendThreadFunction();
	static DWORD WINAPI UDPRecvThread(LPVOID arg);
	void _UDPRecvThreadFunction();

	static DWORD WINAPI ManagerThread(LPVOID arg);
	void _ManagerLoop();

private:
	void __Termination();
	void __Announcement();
	void __Status();
};
