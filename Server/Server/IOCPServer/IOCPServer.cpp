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
	//	// 프로토콜만 보낼때를 제외하고!
	//	if (InDataSize > sizeof(int) && ptr->dataBuffer != nullptr)
	//	{
	//		memcpy(ptr->buf + 4, (char*)ptr->dataBuffer, InDataSize - sizeof(int));
	//		ZeroMemory(&ptr->overlapped, sizeof(ptr->overlapped));
	//
	//		// 잘가랏 고생했다
	//		if (ptr->dataBuffer != nullptr)
	//			delete (ptr->dataBuffer);
	//	}
	//
	//	ptr->dataSize = InDataSize;
	//
	//	// 데이터 바인드
	//	ptr->wsabuf.buf = ptr->buf;
	//	ptr->wsabuf.len = ptr->dataSize;
	//
	//	// 다음 Thread의 처리를 위한 정보 저장
	//	ptr->bufferProtocol = InNextProtocol;
	//	ptr->isRecvTrue = InIsRecvTrue;
	//
	//	// 받아랏!!!
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

		// 데이터 바인드
		ptr->wsabuf.buf = ptr->buf;
		ptr->wsabuf.len = ptr->dataSize;

		std::cout << "  DEBUG - 전송한 Protocol은 : " << reinterpret_cast<int&>(ptr->buf[0]) << "\n";

		// 받아랏!!!
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
		std::cout << "[서버 오류] : 서버의 ExternalIP를 확인할 수 없습니다. " << "\n";
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
		std::cout << "[서버 오류] : 서버의 ExternalIP를 확인할 수 없습니다. " << "\n";
		std::exit(EXIT_FAILURE);
	}
}

void IOCPServer::_PrintServerInfoUI(const bool& InIsTrueLoadExternalIP)
{
	char* retIPChar;
	retIPChar = new char[20]; // IPv4가 20 char보다 클일 죽어도 없음.
	_GetExternalIP(retIPChar);

	printf("\n■■■■■■■■■■■■■■■■■■■■■■■■■\n");
	printf("■ IOCP Server  - Team HSLD My Planet Server    \n");
	printf("■                                ver 2.3 181126\n");
	printf("■\n");

	if (InIsTrueLoadExternalIP) {
		printf("■    IP Address : ExternalIP(%s) \n", retIPChar);

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

				while (pBuffer != NULL)               // 자른 문자열이 나오지 않을 때까지 반복
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
		printf("■    IP Address : LocalHost(127.0.0.1) \n");
	}

	printf("■    Server Port : %d \n", SERVER_PORT);
	printf("■■■■■■■■■■■■■■■■■■■■■■■■■\n\n");

	delete[]retIPChar;
}

//void IOCPServer::LoadUserData()
//{
//	userData.Load();
//}

void IOCPServer::_InitWinSocket()
{
	//윈속 초기화
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		std::cout << "[서버 오류] : 윈속 초기화 실패로 인한 종료. " << "\n";
		exit(EXIT_FAILURE);
	}

	// 입출력 완료 포트 생성
	hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	/*
	CreateIoCompletionPort는 두가지 역할을 함!
	1. 입출력 완료 포트 생성
	2. 소켓과 입출력 완료 포트 연결 (IO장치와 IOCP연결)

	1번째 인자값,  IOCP와 연결할 핸들, 생성시는 INVALID_HANDLE_VALUE를 인자로 넘김
	2번째 인자값,  IOCP 핸들, 첫 생성시는 NULL
	3번째 인자값, IO완료시 넘어갈 값, 사용자가 넘기고 싶은 값 넘김
	4번째 인자값, 한번에 동작할 최대 스레드 개수, 0 넘기면 프로세스 숫자로 자동 지정됨
	*/

	if (hIOCP == NULL)
	{
		std::cout << "[서버 오류] : 입출력 완료 포트 생성 실패로 인한 종료. " << "\n";
		exit(EXIT_FAILURE);
	}

	// CPU 개수 확인
	SYSTEM_INFO si;
	GetSystemInfo(&si);

	// 지금 내 CPU 개수 == 12개 * 2개의 작업자 스레드 생성
	// IO작업이 완료된 후, 완료된 IO에 대한 처리를 수행할 스레드 풀을 구성한다.
	// 일반적으로 스레드 풀의 크기는 프로세서 개수의 2배 정도를 할당한다.
	// 하지만 과연 이 2배라는 수가 내가 만든 서버에서 가장 최적의 성능을 낼 수 있다고 보장할 수 있는 것일까..
	HANDLE hThread;
	//for (int i = 0; i < (int)si.dwNumberOfProcessors * 2; ++i)

	for (int i = 0; i < (int)2; ++i)	// 현재 2 쓰레드만 사용.
	{
		hThread = CreateThread(NULL, 0, WorkerThread, (LPVOID)this, 0, NULL);
		if (hThread == NULL)
		{
			std::cout << "[서버 오류] : WorkerThread 생성 실패로 인한 종료. " << "\n";
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

		// 클라이언트 서버에 접속(Accept) 함을 알림
		//printf("[TCP 서버] 클라이언트 접속 : IP 주소 =%s, Port 번호 = %d \n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

		// 소켓과 입출력 완료 포트 연결
		CreateIoCompletionPort((HANDLE)clientSocket, hIOCP, clientSocket, 0);

		// 소켓 정보 구조체 할당
		SocketInfo *pClient = new SocketInfo;
		if (pClient == nullptr)
		{
			std::cout << "[서버 오류] : Accept한 Client가 nullptr으로 AcceptThread 종료." << "\n";
			break;
		}

		ZeroMemory(&pClient->overlapped, sizeof(pClient->overlapped));

		pClient->sock = clientSocket;
		pClient->isRecvTurn = true;

		pClient->wsabuf.buf = pClient->buf;
		pClient->wsabuf.len = BUF_SIZE;

		// 비동기 입출력의 시작
		flags = 0;
		retValBuffer = WSARecv(
			clientSocket, // 클라이언트 소켓
			&pClient->wsabuf, // 읽을 데이터 버퍼의 포인터
			1,			 // 데이터 입력 버퍼의 개수
			&recvBytes,  // recv 결과 읽은 바이트 수, IOCP에서는 비동기 방식으로 사용하지 않으므로 nullPtr를 넘겨도 무방
			&flags,		 // recv에 사용될 플래그
			&pClient->overlapped, // overlapped구조체의 포인터
			NULL			// IOCP에서는 사용하지 않으므로 NULL, nullptr넘겨도 무방
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

	//....? 우리 열심히 일한 불쌍한 쓰레드들은 어떻게 하나,, 전능하신 운영체제께서 알아서..ㅎ 
	// 임마 쇼하지 말고 나중에...초반에 핸들값들 다 벡터에 차곡차곡 모아서 여기서 멈추게해드려..
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
	// 한 번만 선언해서 여러번 씁시다. 아껴써야지...
	int retVal{};
	int recvType{};
	DWORD flags = 0;
	DWORD recvBytes{};
	DWORD sendBytes{};

	while (7)
	{
		//std::cout << "Wait Threa";
#pragma region [ Wait For Thread ]
		//비동기 입출력 기다리기
		DWORD cbTransferred;
		SOCKET clientSocket;
		SocketInfo *pClient;

		// 입출력 완료 포트에 저장된 결과를 처리하기 위한 함수 // 대기 상태가 됨
		retVal = GetQueuedCompletionStatus(
			hIOCP, //입출력 완료 포트 핸들
			&cbTransferred, //비동기 입출력 작업으로, 전송된 바이트 수가 여기에 저장된다.
			&clientSocket, //함수 호출 시 전달한 세번째 인자(32비트) 가 여기에 저장된다.
			(LPOVERLAPPED *)&pClient, //Overlapped 구조체의 주소값
			INFINITE // 대기 시간 -> 깨울떄 까지 무한대
		);
#pragma endregion
		//std::cout << "Thread Fire!!" << std::endl;
#pragma region [ Get Socket and error Exception ]

		// 할당받은 소켓 즉! 클라이언트 정보 얻기
		SOCKADDR_IN clientAddr;
		int addrLength = sizeof(clientAddr);
		getpeername(pClient->sock, (SOCKADDR *)&clientAddr, &addrLength);

		//비동기 입출력 결과 확인 // 아무것도 안보낼 때는, 해당 클라이언트 접속에 문제가 생긴것으로 판단, 닫아버리겠다!
		// 근데 이거 에코서버일떄만 그래야되는거 아니야???? 몰봐 임마 뭘봐 모를수도 있지
		ERROR_CLIENT_DISCONNECT:
		if (retVal == 0 || cbTransferred == 0)
		{
#ifdef _DEBUG
			if(pClient != nullptr && pClient->pUserNode != nullptr)
				std::cout << "     [UserDataManager] 클라이언트 종료 혹은 연결 끊김 Id = " << pClient->pUserNode->GetKey() << std::endl;
			//printf("[TCP 서버] 클라이언트 종료 : IP 주소 =%s, 포트 번호 =%d\n",
			//	inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
#endif
			// 게임 방 접속 여부 확인. // 방 나갈 경우 해당 조건 False 필요.
			if (pClient->pRoomIter != nullptr)
			{
				if (pClient->pRoomIter->GetGamePlay())	// 게임중일 때,
				{
					pUserData->SetGameResult(pClient->pUserNode, false);
					pClient->pRoomIter->SetDataProtocol(pClient->isHost, DISCONNECTED_ENEMY_CLIENT);
					pClient->pRoomIter.reset();
				}
				else
				{
					if (pClient->pRoomIter->GetIsFriendMode())	// 친구모드일 때,
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
						std::cout << " 이상해요! " << std::endl;
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
				std::cout << pClient->pUserNode->GetID() << " 에게 받은 타입은 : " << recvType << " 입니다. \n";
			else 
				std::cout << "익명의 클라이언트에게 받은 타입은 : " << recvType << " 입니다. \n";
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
		std::cout << "\n [ManagerCommand] 0) 종료 1) 전체공지 : ";
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

	std::cout << "\n [ManagerCommand] 전송할 메세지를 입력해주세요. : ";
	std::cin >> localAnnounceString;

	pUdpManager->SetAnnounceString(CONVERT_UTIL::StringToWString(localAnnounceString));
	pUserData->TraversalForAnnouncement(pUdpManager);
}
