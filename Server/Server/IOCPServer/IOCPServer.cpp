#include "../PCH/stdafx.h"
#include "IOCPServer.h"

/*

 1. InGameScene ���߿� �� ������ �ű�� ( if 500 <=  hi  < 600 ) �Լ������ͷ� �ؽ��ع�����.

*/

// Server's global Function
namespace NETWORK_UTIL {
	void ERROR_QUIT(char *msg)
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

	void ERROR_DISPLAY(char *msg)
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

	//bool SendProcess(SOCKETINFO* ptr, const int InDataSize, const int InNowProtocol, const Protocol InNextProtocol = END_SEND, const bool InIsRecvTrue = true)
	//{
	//	memcpy(ptr->buf, (char*)&InNowProtocol, sizeof(int));
	//
	//	// �������ݸ� �������� �����ϰ�!
	//	if (InDataSize > sizeof(int) && ptr->dataBuffer != nullptr)
	//	{
	//		memcpy(ptr->buf + 4, (char*)ptr->dataBuffer, InDataSize - sizeof(int));
	//		ZeroMemory(&ptr->overlapped, sizeof(ptr->overlapped));
	//
	//		// �߰��� ����ߴ�
	//		if (ptr->dataBuffer != nullptr)
	//			delete (ptr->dataBuffer);
	//	}
	//
	//	ptr->dataSize = InDataSize;
	//
	//	// ������ ���ε�
	//	ptr->wsabuf.buf = ptr->buf;
	//	ptr->wsabuf.len = ptr->dataSize;
	//
	//	// ���� Thread�� ó���� ���� ���� ����
	//	ptr->bufferProtocol = InNextProtocol;
	//	ptr->isRecvTrue = InIsRecvTrue;
	//
	//	// �޾ƶ�!!!
	//	int retVal = WSASend(ptr->sock, &ptr->wsabuf, 1, NULL, 0, &ptr->overlapped, NULL);
	//
	//	if (retVal == SOCKET_ERROR)
	//	{
	//		if (WSAGetLastError() != WSA_IO_PENDING)
	//		{
	//			ERROR_DISPLAY((char *)"WSASend()");
	//		}
	//		return true;
	//	}
	//}

	bool SendProcess(SocketInfo* ptr)
	{
		ZeroMemory(&ptr->overlapped, sizeof(ptr->overlapped));

		ptr->isRecvTurn = false;

		// ������ ���ε�
		ptr->wsabuf.buf = ptr->buf;
		ptr->wsabuf.len = ptr->dataSize;

		// �޾ƶ�!!!
		int retVal = WSASend(ptr->sock, &ptr->wsabuf, 1, NULL, 0, &ptr->overlapped, NULL);

		if (retVal == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSA_IO_PENDING)
			{
				ERROR_DISPLAY((char *)"WSASend()");
			}
			return true;
		}
		return true;
	}
};

//Init
int IOCPServer::_GetExternalIP(char *ip)
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

void IOCPServer::_PrintServerInfoUI()
{
	char* retIPChar;
	retIPChar = new char[20]; // IPv4�� 20 char���� Ŭ�� �׾ ����.
	_GetExternalIP(retIPChar);

	printf("��������������������������\n");
	printf("�� IOCP Server  - Team HSLD My Planet Server    \n");
	printf("��                                ver 2.0 181101\n");
	printf("��\n");
	printf("��    IP Address : %s \n", retIPChar);
	printf("��    Server Port : %d \n", SERVER_PORT);
	printf("��������������������������\n\n");

	delete[]retIPChar;
}

//void IOCPServer::LoadUserData()
//{
//	userData.Load();
//}

void IOCPServer::_InitWinSocket()
{
	//���� �ʱ�ȭ
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("���� �ʱ�ȭ ����");
		exit(1);
	}

	// ����� �Ϸ� ��Ʈ ����
	hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	/*
	CreateIoCompletionPort�� �ΰ��� ������ ��!
	1. ����� �Ϸ� ��Ʈ ����
	2. ���ϰ� ����� �Ϸ� ��Ʈ ���� (IO��ġ�� IOCP����)

	1��° ���ڰ�,  IOCP�� ������ �ڵ�, �����ô� INVALID_HANDLE_VALUE�� ���ڷ� �ѱ�
	2��° ���ڰ�,  IOCP �ڵ�, ù �����ô� NULL
	3��° ���ڰ�, IO�Ϸ�� �Ѿ ��, ����ڰ� �ѱ�� ���� �� �ѱ�
	4��° ���ڰ�, �ѹ��� ������ �ִ� ������ ����, 0 �ѱ�� ���μ��� ���ڷ� �ڵ� ������
	*/

	if (hIOCP == NULL)
	{
		printf("����� �Ϸ� ��Ʈ ���� ����");
		exit(1);
	}

	// CPU ���� Ȯ��
	SYSTEM_INFO si;
	GetSystemInfo(&si);

	// ���� �� CPU ���� == 12�� * 2���� �۾��� ������ ����
	// IO�۾��� �Ϸ�� ��, �Ϸ�� IO�� ���� ó���� ������ ������ Ǯ�� �����Ѵ�.
	// �Ϲ������� ������ Ǯ�� ũ��� ���μ��� ������ 2�� ������ �Ҵ��Ѵ�.
	// ������ ���� �� 2���� ���� ���� ���� �������� ���� ������ ������ �� �� �ִٰ� ������ �� �ִ� ���ϱ�..
	HANDLE hThread;
	//for (int i = 0; i < (int)si.dwNumberOfProcessors * 2; ++i)
	for (int i = 0; i < (int)2; ++i)
	{
		hThread = CreateThread(NULL, 0, WorkerThread, (LPVOID)this, 0, NULL);
		if (hThread == NULL)
		{
			printf("�۾� ������ ���� ��, Null");
			exit(1);
		}
		CloseHandle(hThread);
	}
}

void IOCPServer::_CreateBindListen()
{
	//Socket()
	listenSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (listenSocket == INVALID_SOCKET) NETWORK_UTIL::ERROR_QUIT((char *)"socket()");

	//bind()
	ZeroMemory(&serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = htons(SERVER_PORT);
	int retVal = ::bind(listenSocket, (SOCKADDR *)&serverAddr, sizeof(serverAddr));
	if (retVal == SOCKET_ERROR) NETWORK_UTIL::ERROR_QUIT((char *)"bind()");

	// Listen()!
	retVal = listen(listenSocket, SOMAXCONN);
	if (retVal == SOCKET_ERROR) NETWORK_UTIL::ERROR_QUIT((char *)"listen()");
}

void IOCPServer::_BindSceneDataProcess()
{
	sceneNetworkManagerArr[0] = new SCENE_NETWORK_MANAGER::TitleScene;
	sceneNetworkManagerArr[1] = new SCENE_NETWORK_MANAGER::LoginScene;
	sceneNetworkManagerArr[2] = new SCENE_NETWORK_MANAGER::MainUiScene;
	sceneNetworkManagerArr[3] = new SCENE_NETWORK_MANAGER::LobbyScene;
	sceneNetworkManagerArr[4] = new SCENE_NETWORK_MANAGER::RoomScene;
	sceneNetworkManagerArr[5] = new SCENE_NETWORK_MANAGER::InGameScene(&isSaveOn);

	//sceneArr.reserve(6);
	//
	//sceneArr.emplace_back(new SCENE_NETWORK_MANAGER::TitleScene);
	//sceneArr.emplace_back(new SCENE_NETWORK_MANAGER::LoginScene);
	//sceneArr.emplace_back(new SCENE_NETWORK_MANAGER::MainUiScene);
	//sceneArr.emplace_back(new SCENE_NETWORK_MANAGER::LobbyScene);
	//sceneArr.emplace_back(new SCENE_NETWORK_MANAGER::RoomScene);
	//sceneArr.emplace_back(new SCENE_NETWORK_MANAGER::InGameScene);

	//SceneDataProcess[0] = &(sceneArr[0].ProcessData);
	//SceneDataProcess[1] = &(sceneArr[1].ProcessData);
	//SceneDataProcess[2] = &(sceneArr[2].ProcessData);
	//SceneDataProcess[3] = &(sceneArr[3].ProcessData);
	//SceneDataProcess[4] = &(sceneArr[4].ProcessData);
	//SceneDataProcess[5] = &(sceneArr[5].ProcessData);

	//SceneDataProcess[0] = titleScene.ProcessData;
	//SceneDataProcess[1] = loginScene.ProcessData;
	//SceneDataProcess[2] = mainUiScene.ProcessData;
	//SceneDataProcess[3] = lobbyScene.ProcessData;
	//SceneDataProcess[4] = roomScene.ProcessData;
	//SceneDataProcess[5] = inGameScene.ProcessData;
}

void IOCPServer::_CreateUDPSocket()
{
	udpSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if (udpSocket == INVALID_SOCKET) NETWORK_UTIL::ERROR_QUIT((char *)"Socket()");

	ZeroMemory(&serverUDPAddr, sizeof(serverUDPAddr));
	serverUDPAddr.sin_family = AF_INET;
	serverUDPAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverUDPAddr.sin_port = htons(SERVER_UDP_PORT);

	if (int retval = ::bind(udpSocket, (SOCKADDR *)&serverUDPAddr, sizeof(serverUDPAddr)); 
	retval == SOCKET_ERROR)
		NETWORK_UTIL::ERROR_QUIT((char *)"bind()");
}

//Run
void IOCPServer::_AcceptProcess()
{
	printf("     [ServerCore] Dedicated server activated!\n\n");

	SOCKET clientSocket;
	SOCKADDR_IN clientAddr;
	int addrLength;
	DWORD recvBytes, flags;
	int retValBuffer;

	while (7) {
		//accept()
		addrLength = sizeof(clientAddr);
		clientSocket = accept(listenSocket, (SOCKADDR *)&clientAddr, &addrLength);
		if (clientSocket == INVALID_SOCKET)
		{
			NETWORK_UTIL::ERROR_DISPLAY((char *)"accept()");
			break;
		}

		// Ŭ���̾�Ʈ ������ ����(Accept) ���� �˸�
		//printf("[TCP ����] Ŭ���̾�Ʈ ���� : IP �ּ� =%s, Port ��ȣ = %d \n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

		// ���ϰ� ����� �Ϸ� ��Ʈ ����
		CreateIoCompletionPort((HANDLE)clientSocket, hIOCP, clientSocket, 0);

		// ���� ���� ����ü �Ҵ�
		SocketInfo *ptr = new SocketInfo;
		if (ptr == NULL) break;

		ZeroMemory(&ptr->overlapped, sizeof(ptr->overlapped));

		ptr->sock = clientSocket;
		ptr->isRecvTurn = true;

		ptr->wsabuf.buf = ptr->buf;
		ptr->wsabuf.len = BUF_SIZE;

		// �񵿱� ������� ����
		flags = 0;
		retValBuffer = WSARecv(
			clientSocket, // Ŭ���̾�Ʈ ����
			&ptr->wsabuf, // ���� ������ ������ ������
			1,			 // ������ �Է� ������ ����
			&recvBytes,  // recv ��� ���� ����Ʈ ��, IOCP������ �񵿱� ������� ������� �����Ƿ� nullPtr�� �Ѱܵ� ����
			&flags,		 // recv�� ���� �÷���
			&ptr->overlapped, // overlapped����ü�� ������
			NULL			// IOCP������ ������� �����Ƿ� NULL, nullptr�Ѱܵ� ����
		);

		if (retValBuffer == SOCKET_ERROR)
		{
			if (WSAGetLastError() != ERROR_IO_PENDING)
			{
				NETWORK_UTIL::ERROR_DISPLAY((char *)"WSARecv()");
			}

			continue;
		}
	}
}

void IOCPServer::_RunManagerThread()
{
	hManagerThread = CreateThread(NULL, 0, ManagerThread, (LPVOID)this, 0, NULL);
	//CloseHandle(hSaveUserDataThread);
	std::cout << "     [UserDataManager] Run Manager Thread! " << "\n";
}

void IOCPServer::_RunTestThread()
{
	hTestThread = CreateThread(NULL, 0, ManagerThread, (LPVOID)this, 0, NULL);
	std::cout << "     [UserDataManager] Run Test Thread! " << "\n";
}

//Close
void IOCPServer::_DestroyAndClean()
{
	closesocket(listenSocket);
	WSACleanup();
	//....? �츮 ������ ���� �ҽ��� ��������� ��� �ϳ�,, �����Ͻ� �ü������ �˾Ƽ�..�� 
	// �Ӹ� ������ ���� ���߿�...�ʹݿ� �ڵ鰪�� �� ���Ϳ� �������� ��Ƽ� ���⼭ ���߰��ص��..
	isSaveOn = true;

	while (7) 
	{
		if (!isSaveOn) 
		{
			printf(" [System] User Data Last Save Success! \n\n");
			CloseHandle(hManagerThread);
		}
	}

}


//thread's
DWORD WINAPI IOCPServer::WorkerThread(LPVOID arg)
{
	IOCPServer* server = static_cast<IOCPServer*>(arg);
	server->_WorkerThreadFunction();

	return 0;
};

void IOCPServer::_WorkerThreadFunction()
{
	// �� ���� �����ؼ� ������ ���ô�. �Ʋ������...
	int retVal{};
	int recvType{};
	DWORD flags = 0;
	DWORD recvBytes{};
	DWORD sendBytes{};

	while (7)
	{
		//std::cout << "Wait Threa";
#pragma region [ Wait For Thread ]
		//�񵿱� ����� ��ٸ���
		DWORD cbTransferred;
		SOCKET clientSocket;
		SocketInfo *pClient;

		// ����� �Ϸ� ��Ʈ�� ����� ����� ó���ϱ� ���� �Լ� // ��� ���°� ��
		retVal = GetQueuedCompletionStatus(
			hIOCP, //����� �Ϸ� ��Ʈ �ڵ�
			&cbTransferred, //�񵿱� ����� �۾�����, ���۵� ����Ʈ ���� ���⿡ ����ȴ�.
			&clientSocket, //�Լ� ȣ�� �� ������ ����° ����(32��Ʈ) �� ���⿡ ����ȴ�.
			(LPOVERLAPPED *)&pClient, //Overlapped ����ü�� �ּҰ�
			INFINITE // ��� �ð� -> ��� ���� ���Ѵ�
		);
#pragma endregion
		//std::cout << "Thread Fire!!" << std::endl;
#pragma region [ Get Socket and error Exception ]

		// �Ҵ���� ���� ��! Ŭ���̾�Ʈ ���� ���
		SOCKADDR_IN clientAddr;
		int addrLength = sizeof(clientAddr);
		getpeername(pClient->sock, (SOCKADDR *)&clientAddr, &addrLength);

		//�񵿱� ����� ��� Ȯ�� // �ƹ��͵� �Ⱥ��� ����, �ش� Ŭ���̾�Ʈ ���ӿ� ������ ��������� �Ǵ�, �ݾƹ����ڴ�!
		// �ٵ� �̰� ���ڼ����ϋ��� �׷��ߵǴ°� �ƴϾ�???? ���� �Ӹ� ���� �𸦼��� ����
		if (retVal == 0 || cbTransferred == 0)
		{
#ifdef _DEBUG
			std::cout << "     [UserDataManager] Ŭ���̾�Ʈ ���� Ȥ�� ���� ���� Id = " << pClient->pUserNode->GetKey() << std::endl;
			//printf("[TCP ����] Ŭ���̾�Ʈ ���� : IP �ּ� =%s, ��Ʈ ��ȣ =%d\n",
			//	inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
#endif
			// ���� �� ���� ���� Ȯ��. // �� ���� ��� �ش� ���� False �ʿ�.
			if (pClient->pRoomIter != nullptr)
			{
				//if (map<const std::string, UserData>::iterator enemyIter; 
				//roomData.SignOut(ptr->roomIndex, ptr->isHost, ReturnEnemyIndexBuffer))
				//{
				//	// ���� ���̿��� ���, ���� Ŭ���̾�Ʈ�� �й� ó�� ��, ��� Ŭ���̾�Ʈ �¸� ó��
				//	userData.SetGameResult(ReturnEnemyIndexBuffer, true);
				//	userData.SetGameResult(ptr->userIndex, false);
				//}
				//else
				//{
				//	// ���� ���� �ƴϿ��ų�, ������ �ٸ� �� ��. 
				//	//...? �Ұ� ����...?
				//}
				userData.SetGameResult(pClient->pUserNode, false);
				pClient->pRoomIter->SetDataProtocol(pClient->isHost, DISCONNECTED_ENEMY_CLIENT);
				pClient->pRoomIter = nullptr;
			}

			userData.LogoutProcess(pClient);

			//std::cout << "DEBUG - Error or Exit Client A" << std::endl;
			if (retVal == 0)
			{
				DWORD temp1, temp2;
				WSAGetOverlappedResult(pClient->sock, &pClient->overlapped, &temp1, FALSE, &temp2);
				NETWORK_UTIL::ERROR_DISPLAY((char *)"WSAGetOverlappedResult()");
			}
			closesocket(pClient->sock);

			delete pClient;
			continue;
		}

#pragma endregion

		if (!pClient->isRecvTurn)
		{
			pClient->isRecvTurn = true;

			pClient->wsabuf.buf = pClient->buf;
			pClient->wsabuf.len = BUF_SIZE;

			retVal = WSARecv(clientSocket, &pClient->wsabuf, 1, &recvBytes, &flags, &pClient->overlapped, NULL);

			if (retVal == SOCKET_ERROR)
			{
				if (WSAGetLastError() != ERROR_IO_PENDING)
				{
					NETWORK_UTIL::ERROR_DISPLAY((char *)"WSARecv()");
				}
				continue;
			}
		}
		else
		{
			recvType = (int&)(pClient->buf);
		
			std::cout << " [] ���� Ÿ���� : " << recvType << " �Դϴ�. \n";
			//SceneDataProcess[static_cast<int>(recvType * 0.01)](recvType, ptr, roomData, userData);
			//sceneArr[1]->ProcessData(recvType, *ptr, roomData, userData);

			sceneNetworkManagerArr[static_cast<int>(recvType * 0.01)]->ProcessData(recvType, pClient, roomData, userData);

			if (NETWORK_UTIL::SendProcess(pClient))
				continue;
		}
	}
}

DWORD WINAPI IOCPServer::ManagerThread(LPVOID arg)
{
	IOCPServer* server = static_cast<IOCPServer*>(arg);
	
	server->_ManagerLoop();

	return 0;
}

void IOCPServer::_ManagerLoop()
{
	isSaveOn = false;
	isSendUDPMessage = false;

	while (777)
	{
		Sleep(1000);

		//if (isSaveOn) SaveUserData();
		//
		//Sleep(1000);

		if (isSendUDPMessage)_SendDynamicMessage();

		Sleep(1000);
	}
}

DWORD WINAPI IOCPServer::TestThread(LPVOID arg)
{
	IOCPServer* server = static_cast<IOCPServer*>(arg);

	server->_TestThreadFunction();

	return 0;
}

void IOCPServer::_TestThreadFunction()
{
	int buffer;

	udpMessageBuffer = 'a';

	while (7)
	{
		std::cout << "��� �Է� : ";
		std::cin >> buffer;

		if (buffer == 0) break;
		else if (buffer == 1) _SendDynamicMessage();
	}
}

// UDP Function
void IOCPServer::_SendDynamicMessage()
{
	bool isSendTrue = false;
	rbTreeNode<string, UserData>* pUdpClient = userData.SearchUserNode("abcd", isSendTrue);

	if (isSendTrue)
	{
		SOCKADDR_IN clientAddr;
		int addrLength = sizeof(clientAddr);

		getpeername(pUdpClient->SetValue().GetSocketInfo()->sock, (SOCKADDR *)&clientAddr, &addrLength);
		clientAddr.sin_port = htons(SERVER_UDP_PORT);

		udpMessageBuffer[0] = 1;

		// !! for (UserData) for GetPeerName
		sendto(udpSocket, udpMessageBuffer.data(), 1, 0, (sockaddr*)&clientAddr, sizeof(clientAddr));

		isSendUDPMessage = false;
	}
}

//void IOCPServer::SaveUserData()
//{
//		//���� �ھߵǴϱ� ���߿� �̰ź��� 
//		//isSaveOn == if�� �Լ� ������ ���� ��û��... �̰� �ŵ��� ����־��פѤ�
//		// ? ���۷����� �ȿ��� �ٲ��ٰǵ� �ٺ�?
//		//userData.Save(isSaveOn);
//}