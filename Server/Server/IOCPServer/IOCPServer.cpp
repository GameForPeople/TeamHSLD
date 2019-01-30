#include "../PCH/stdafx.h"
#include "IOCPServer.h"

#import "GetServerIP.tlb" no_namespace named_guids

#pragma warning (disable: 4278)

// Server's global Function
namespace NETWORK_UTIL {
	_NORETURN void ERROR_QUIT(const char *msg)
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

	_NORETURN void ERROR_DISPLAY(const char *msg)
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
		
		printf(" [%s]  %s", msg, (LPTSTR)&lpMsgBuf);
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

		std::cout << "  DEBUG - ������ Protocol�� : " << reinterpret_cast<int&>(ptr->buf[0]) << "\n";

		// �޾ƶ�!!!
		int retVal = WSASend(ptr->sock, &ptr->wsabuf, 1, NULL, 0, &ptr->overlapped, NULL);
		if (retVal == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSA_IO_PENDING)
			{
				ERROR_DISPLAY(TEXT("WSASend()"));
			}
			return true;
		}
		return true;
	}
};

//Init
void IOCPServer::_GetExternalIP(char *ip)
{
	HINTERNET hInternet, hFile;
	DWORD rSize;
	
	char buffer[256] = { 0 };

	hInternet = InternetOpen(NULL, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);

	if (hInternet == NULL)
	{
		std::cout << "[���� ����] : ������ ExternalIP�� Ȯ���� �� �����ϴ�. " << "\n";
		std::exit(EXIT_FAILURE);
	}

	hFile = InternetOpenUrl(hInternet, EXTERNALIP_FINDER_URL, NULL, 0, INTERNET_FLAG_RELOAD, 0);

	if (hFile)
	{
		InternetReadFile(hFile, &buffer, sizeof(buffer), &rSize);
		buffer[rSize] = '\0';

		int nShift = _tcslen(TITLE_PARSER);
		string strHTML = buffer;
		string::size_type nIdx = strHTML.find(TITLE_PARSER);
		strHTML.erase(strHTML.begin(), strHTML.begin() + nIdx + nShift);
		nIdx = strHTML.find(TEXT("</body>"));
		strHTML.erase(strHTML.begin() + nIdx, strHTML.end());

		_tcscpy(ip, strHTML.c_str());
		InternetCloseHandle(hFile);
		InternetCloseHandle(hInternet);

		// return true; //_tcslen(ip);
	}
	else
	{
		std::cout << "[���� ����] : ������ ExternalIP�� Ȯ���� �� �����ϴ�. " << "\n";
		std::exit(EXIT_FAILURE);
	}
}

void IOCPServer::_PrintServerInfoUI(const bool& InIsTrueLoadExternalIP)
{
	char* retIPChar;
	retIPChar = new char[20]; // IPv4�� 20 char���� Ŭ�� �׾ ����.
	_GetExternalIP(retIPChar);

	printf("\n��������������������������\n");
	printf("�� IOCP Server  - Team HSLD My Planet Server    \n");
	printf("��                                ver 2.3 181126\n");
	printf("��\n");

	if (InIsTrueLoadExternalIP) {
		printf("��    IP Address : ExternalIP(%s) \n", retIPChar);

		// For Test! Extern IP == HSLD WebServer IP
		[&retIPChar]()  noexcept(false) -> void 
		{
			NetworkInterface_CS* networkInterFace = nullptr;
			CoInitialize(NULL);

			HRESULT hResult = CoCreateInstance(CLSID_NetworkClass_CS,
				NULL, CLSCTX_INPROC_SERVER,
				IID_NetworkInterface_CS,
				reinterpret_cast<void**>(&networkInterFace));

			if (SUCCEEDED(hResult))
			{
				long retIpAddr[4]{};
				networkInterFace->ParsingServerIP(&retIpAddr[0], &retIpAddr[1], &retIpAddr[2], &retIpAddr[3]);

				char *pBuffer = strtok(retIPChar, "."); 

				long valueBuffer[4]{ 0 };
				int  indexBuffer{ 0 };

				while (pBuffer != NULL)               // �ڸ� ���ڿ��� ������ ���� ������ �ݺ�
				{
					valueBuffer[indexBuffer++] = atoi(pBuffer);
					pBuffer = strtok(NULL, "."); 
				}

				bool isIPSame{ true };
				for (int i = 0; i < 4; ++i)
				{
					if (retIpAddr[i] != valueBuffer[i])
						isIPSame = false;
				}

				if (!isIPSame)
				{
					std::cout << "PARSING_SERVER_IP_ERROR : Please Change HSLD Web Server IP \n";
					throw ERROR;
				}
			}
			else
			{
				std::cout << " ... \n";
				std::cout << " ... \n";
				std::cout << " ... \n";
				std::cout << " [SERVER_ERROR] PARSING_SERVER_IP_ERROR : Parsing Fail \n";
				throw ERROR;
			}

			//delete networkInterFace;
		}();
	}
	else {
		printf("��    IP Address : LocalHost(127.0.0.1) \n");
	}

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
		std::cout << "[���� ����] : ���� �ʱ�ȭ ���з� ���� ����. " << "\n";
		exit(EXIT_FAILURE);
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
		std::cout << "[���� ����] : ����� �Ϸ� ��Ʈ ���� ���з� ���� ����. " << "\n";
		exit(EXIT_FAILURE);
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

	for (int i = 0; i < (int)2; ++i)	// ���� 2 �����常 ���.
	{
		hThread = CreateThread(NULL, 0, WorkerThread, (LPVOID)this, 0, NULL);
		if (hThread == NULL)
		{
			std::cout << "[���� ����] : WorkerThread ���� ���з� ���� ����. " << "\n";
			exit(EXIT_FAILURE);
		}
		CloseHandle(hThread);
	}
}

void IOCPServer::_CreateBindListen()
{
	//Socket()
	listenSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (listenSocket == INVALID_SOCKET) NETWORK_UTIL::ERROR_QUIT(TEXT("socket()"));

	//bind()
	ZeroMemory(&serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = htons(SERVER_PORT);
	int retVal = ::bind(listenSocket, (SOCKADDR *)&serverAddr, sizeof(serverAddr));
	if (retVal == SOCKET_ERROR) NETWORK_UTIL::ERROR_QUIT(TEXT("bind()"));

	// Listen()!
	retVal = listen(listenSocket, SOMAXCONN);
	if (retVal == SOCKET_ERROR) NETWORK_UTIL::ERROR_QUIT(TEXT("listen()"));
}

void IOCPServer::_BindSceneDataProcess()
{
	sceneNetworkManagerCont.reserve(6);	//	Scene Count = 6;

	sceneNetworkManagerCont.push_back(make_unique<SCENE_NETWORK_MANAGER::TitleScene>(pRoomData, pUserData, pUdpManager));
	sceneNetworkManagerCont.push_back(make_unique<SCENE_NETWORK_MANAGER::LoginScene>(pRoomData, pUserData, pUdpManager));
	sceneNetworkManagerCont.push_back(make_unique<SCENE_NETWORK_MANAGER::MainUiScene>(pRoomData, pUserData, pUdpManager));
	sceneNetworkManagerCont.push_back(make_unique<SCENE_NETWORK_MANAGER::LobbyScene>(pRoomData, pUserData, pUdpManager));
	sceneNetworkManagerCont.push_back(make_unique<SCENE_NETWORK_MANAGER::RoomScene>(pRoomData, pUserData, pUdpManager));
	sceneNetworkManagerCont.push_back(make_unique<SCENE_NETWORK_MANAGER::InGameScene>(pRoomData, pUserData, pUdpManager));
}

void IOCPServer::_CreateSocket()
{
	pUdpManager->_CreateUDPSocket();
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
			NETWORK_UTIL::ERROR_DISPLAY(TEXT("accept()"));
			break;
		}

		// Ŭ���̾�Ʈ ������ ����(Accept) ���� �˸�
		//printf("[TCP ����] Ŭ���̾�Ʈ ���� : IP �ּ� =%s, Port ��ȣ = %d \n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

		// ���ϰ� ����� �Ϸ� ��Ʈ ����
		CreateIoCompletionPort((HANDLE)clientSocket, hIOCP, clientSocket, 0);

		// ���� ���� ����ü �Ҵ�
		SocketInfo *pClient = new SocketInfo;
		if (pClient == nullptr)
		{
			std::cout << "[���� ����] : Accept�� Client�� nullptr���� AcceptThread ����." << "\n";
			break;
		}

		ZeroMemory(&pClient->overlapped, sizeof(pClient->overlapped));

		pClient->sock = clientSocket;
		pClient->isRecvTurn = true;

		pClient->wsabuf.buf = pClient->buf;
		pClient->wsabuf.len = BUF_SIZE;

		// �񵿱� ������� ����
		flags = 0;
		retValBuffer = WSARecv(
			clientSocket, // Ŭ���̾�Ʈ ����
			&pClient->wsabuf, // ���� ������ ������ ������
			1,			 // ������ �Է� ������ ����
			&recvBytes,  // recv ��� ���� ����Ʈ ��, IOCP������ �񵿱� ������� ������� �����Ƿ� nullPtr�� �Ѱܵ� ����
			&flags,		 // recv�� ���� �÷���
			&pClient->overlapped, // overlapped����ü�� ������
			NULL			// IOCP������ ������� �����Ƿ� NULL, nullptr�Ѱܵ� ����
		);

		if (retValBuffer == SOCKET_ERROR)
		{
			if (WSAGetLastError() != ERROR_IO_PENDING)
			{
				NETWORK_UTIL::ERROR_DISPLAY(TEXT("WSARecv()"));
			}

			continue;
		}
	}
}

void IOCPServer::_RunOtherThread()
{
	hManagerThread = CreateThread(NULL, 0, ManagerThread, (LPVOID)this, 0, NULL);
	//CloseHandle(hSaveUserDataThread);
	std::cout << "     [UserData_Manager] Run Manager Thread! " << "\n";

	hUDPThread = CreateThread(NULL, 0, UDPThread, (LPVOID)this, 0, NULL);
	std::cout << "     [UDP_Manager] Run UDP Manager Thread! " << "\n";
}


//Close
void IOCPServer::_DestroyAndClean()
{
	closesocket(listenSocket);
	WSACleanup();

	//....? �츮 ������ ���� �ҽ��� ��������� ��� �ϳ�,, �����Ͻ� �ü������ �˾Ƽ�..�� 
	// �Ӹ� ������ ���� ���߿�...�ʹݿ� �ڵ鰪�� �� ���Ϳ� �������� ��Ƽ� ���⼭ ���߰��ص��..
}


//thread's
DWORD WINAPI IOCPServer::WorkerThread(LPVOID arg)
{
	IOCPServer* pServer = static_cast<IOCPServer*>(arg);
	pServer->_WorkerThreadFunction();

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
		ERROR_CLIENT_DISCONNECT:
		if (retVal == 0 || cbTransferred == 0)
		{
#ifdef _DEBUG
			if(pClient != nullptr && pClient->pUserNode != nullptr)
				std::cout << "     [UserDataManager] Ŭ���̾�Ʈ ���� Ȥ�� ���� ���� Id = " << pClient->pUserNode->GetKey() << std::endl;
			//printf("[TCP ����] Ŭ���̾�Ʈ ���� : IP �ּ� =%s, ��Ʈ ��ȣ =%d\n",
			//	inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
#endif
			// ���� �� ���� ���� Ȯ��. // �� ���� ��� �ش� ���� False �ʿ�.
			if (pClient->pRoomIter != nullptr)
			{
				if (pClient->pRoomIter->GetGamePlay())	// �������� ��,
				{
					pUserData->SetGameResult(pClient->pUserNode, false);
					pClient->pRoomIter->SetDataProtocol(pClient->isHost, DISCONNECTED_ENEMY_CLIENT);
					pClient->pRoomIter.reset();
				}
				else
				{
					if (pClient->pRoomIter->GetIsFriendMode())	// ģ������� ��,
					{
						pClient->pRoomIter->SetDynamicFriendInviteBuffer();
						pClient->pRoomIter.reset();
					}
					else if(pClient->isHost)
					{
						pRoomData->HostCancelWaiting(pClient);
					}
					else
					{
						std::cout << " �̻��ؿ�! " << std::endl;
						pClient->pRoomIter.reset();
					}
				}
			}

			if (pClient->pUserNode != nullptr) {
				pUserData->LogoutProcess(pClient->pUserNode);
			}
			//std::cout << "DEBUG - Error or Exit Client A" << std::endl;
			if (retVal == 0)
			{
				DWORD temp1, temp2;
				WSAGetOverlappedResult(pClient->sock, &pClient->overlapped, &temp1, FALSE, &temp2);
				NETWORK_UTIL::ERROR_DISPLAY(TEXT("WSAGetOverlappedResult()"));
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
					NETWORK_UTIL::ERROR_DISPLAY(TEXT("WSARecv()"));
				}
				continue;
			}
		}
		else
		{
			recvType = reinterpret_cast<int&>(pClient->buf);
		
			if(pClient->pUserNode != nullptr)
				std::cout << pClient->pUserNode->GetID() << " ���� ���� Ÿ���� : " << recvType << " �Դϴ�. \n";
			else 
				std::cout << "�͸��� Ŭ���̾�Ʈ���� ���� Ÿ���� : " << recvType << " �Դϴ�. \n";
			//SceneDataProcess[static_cast<int>(recvType * 0.01)](recvType, ptr, roomData, userData);
			//sceneArr[1]->ProcessData(recvType, *ptr, roomData, userData);

			if (recvType > 0 && recvType < 600)
			{
				sceneNetworkManagerCont[static_cast<int>(recvType * 0.01)]->ProcessData(recvType, pClient);

				if (NETWORK_UTIL::SendProcess(pClient))
					continue;
			}
			else 
			{
				retVal = 0;
				goto ERROR_CLIENT_DISCONNECT;
			}
		}
	}
}

DWORD WINAPI IOCPServer::UDPThread(LPVOID arg)
{
	IOCPServer* server = static_cast<IOCPServer*>(arg);
	server->_UDPThreadFunction();

	return 0;
};

void IOCPServer::_UDPThreadFunction()
{
	while (7)
	{
		pUdpManager->UDPSend();
		Sleep(1000);
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
	enum /*class*/ MANAGER_COMMAND : int
	{
		TERMINATION = 0,
		ANNOUNCEMENT = 1
	};

	int inputtedManagerCommand{};

	while (7)
	{
		std::cout << "\n [ManagerCommand] 0) ���� 1) ��ü���� : ";
		std::rewind(stdin);
		std::cin >> inputtedManagerCommand;
	
		switch (inputtedManagerCommand)
		{
		case MANAGER_COMMAND::TERMINATION:
			__Termination();
			break;
		case MANAGER_COMMAND::ANNOUNCEMENT:
			__Announcement();
			break;
		}
	}
}

void IOCPServer::__Termination()
{
	
}

void IOCPServer::__Announcement()
{
	string localAnnounceString;

	std::cout << "\n [ManagerCommand] ������ �޼����� �Է����ּ���. : ";
	std::cin >> localAnnounceString;

	pUdpManager->SetAnnounceString(CONVERT_UTIL::StringToWString(localAnnounceString));
	pUserData->TraversalForAnnouncement(pUdpManager);
}
