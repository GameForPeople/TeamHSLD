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
			std::cout << "[���� ����] : IOCPServer �Ҹ��ڿ��� ���ܰ� �߻��߽��ϴ�. (��� ��������!)" << "\n";
			std::exit(EXIT_FAILURE);
		}
	}
	
public:
	// Init Server
	void Init(const SERVER_INIT_TYPE InIsTrueLoadExternalIP)
	{
		// ������ ���� OS ����.
		setlocale(LC_ALL, "Korean");

		_wsetlocale(LC_ALL, L"Korean");

		_PrintServerInfoUI(InIsTrueLoadExternalIP);
		//LoadUserData();	// ���߿� �г��� �ؽ� Ʈ�� ����� �۾� ���⼭ �ʿ���.

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

	void _PrintServerInfoUI(const SERVER_INIT_TYPE InIsTrueLoadExternalIP);	// ���� �����ڿ��� �ʿ��� UI�� ����ݴϴ�.
	
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
