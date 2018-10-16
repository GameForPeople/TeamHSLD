#include "../stdafx.h"
#include "IOCPServer.h"

// in Server global Function
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

	bool SendProcess(SOCKETINFO* ptr, const int InDataSize, const int InNowProtocol, const Protocol InNextProtocol = END_SEND, const bool InIsRecvTrue = true)
	{
		memcpy(ptr->buf, (char*)&InNowProtocol, sizeof(int));

		// 프로토콜만 보낼때를 제외하고!
		if (InDataSize > sizeof(int) && ptr->dataBuffer != nullptr)
		{
			//std::cout << "[DEBUG_MESSAGE] DataSize : " << InDataSize << ",  Protocol : " << InNowProtocol << "\n";

			memcpy(ptr->buf + 4, (char*)ptr->dataBuffer, InDataSize - sizeof(int));
			ZeroMemory(&ptr->overlapped, sizeof(ptr->overlapped));

			// 잘가랏 고생했다
			if (ptr->dataBuffer != nullptr)
				delete (ptr->dataBuffer);
		}

		ptr->dataSize = InDataSize;

		// 데이터 바인드
		ptr->wsabuf.buf = ptr->buf;
		ptr->wsabuf.len = ptr->dataSize;

		// 다음 Thread의 처리를 위한 정보 저장
		ptr->bufferProtocol = InNextProtocol;
		ptr->isRecvTrue = InIsRecvTrue;

		// 받아랏!!!
		int retVal = WSASend(ptr->sock, &ptr->wsabuf, 1, NULL, 0, &ptr->overlapped, NULL);

		if (retVal == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSA_IO_PENDING)
			{
				ERROR_DISPLAY((char *)"WSASend()");
			}
			return true;
		}
	}
};

//Init
int IOCPServer::GetExternalIP(char *ip)
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

void IOCPServer::PrintServerInfoUI()
{
	char* retIPChar;
	retIPChar = new char[20]; // IPv4가 20 char보다 클일 죽어도 없음.
	GetExternalIP(retIPChar);

	printf("■■■■■■■■■■■■■■■■■■■■■■■■■\n");
	printf("■ IOCP Server  - Team HSLD My Planet Server    \n");
	printf("■                                ver 1.3 180914\n");
	printf("■\n");
	printf("■    IP Address : %s \n", retIPChar);
	printf("■    Server Port : %d \n", SERVER_PORT);
	printf("■■■■■■■■■■■■■■■■■■■■■■■■■\n\n");

	delete[]retIPChar;
}

void IOCPServer::LoadUserData()
{
	userData.Load();
}

void IOCPServer::InitWinSocket()
{
	//윈속 초기화
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("윈속 초기화 실패");
		exit(1);
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
		printf("입출력 완료 포트 생성 실패");
		exit(1);
	}

	// CPU 개수 확인
	SYSTEM_INFO si;
	GetSystemInfo(&si);

	// 지금 내 CPU 개수 == 12개 * 2개의 작업자 스레드 생성
	// IO작업이 완료된 후, 완료된 IO에 대한 처리를 수행할 스레드 풀을 구성한다.
	// 일반적으로 스레드 풀의 크기는 프로세서 개수의 2배 정도를 할당한다.
	// 하지만 과연 이 2배라는 수가 내가 만든 서버에서 가장 최적의 성능을 낼 수 있다고 보장할 수 있는 것일까..
	HANDLE hThread;
	for (int i = 0; i < (int)si.dwNumberOfProcessors * 2; ++i)
	{
		hThread = CreateThread(NULL, 0, WorkerThread, (LPVOID)this, 0, NULL);
		if (hThread == NULL)
		{
			printf("작업 스레드 생성 중, Null");
			exit(1);
		}
		CloseHandle(hThread);
	}
}

void IOCPServer::CreateBindListen()
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


//Run
void IOCPServer::AcceptProcess()
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

		// 클라이언트 서버에 접속(Accept) 함을 알림
		printf("[TCP 서버] 클라이언트 접속 : IP 주소 =%s, Port 번호 = %d \n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

		// 소켓과 입출력 완료 포트 연결
		CreateIoCompletionPort((HANDLE)clientSocket, hIOCP, clientSocket, 0);

		// 소켓 정보 구조체 할당
		SOCKETINFO *ptr = new SOCKETINFO;
		if (ptr == NULL) break;

		ZeroMemory(&ptr->overlapped, sizeof(ptr->overlapped));

		ptr->sock = clientSocket;
		ptr->isRecvTrue = true;
		ptr->bufferProtocol = 0;

		ptr->wsabuf.buf = ptr->buf;
		ptr->wsabuf.len = BUF_SIZE;

		// 비동기 입출력의 시작
		flags = 0;
		retValBuffer = WSARecv(
			clientSocket, // 클라이언트 소켓
			&ptr->wsabuf, // 읽을 데이터 버퍼의 포인터
			1,			 // 데이터 입력 버퍼의 개수
			&recvBytes,  // recv 결과 읽은 바이트 수, IOCP에서는 비동기 방식으로 사용하지 않으므로 nullPtr를 넘겨도 무방
			&flags,		 // recv에 사용될 플래그
			&ptr->overlapped, // overlapped구조체의 포인터
			NULL			// IOCP에서는 사용하지 않으므로 NULL, nullptr넘겨도 무방
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


//Close
void IOCPServer::DestroyAndClean()
{
	closesocket(listenSocket);
	WSACleanup();
	//....? 우리 열심히 일한 불쌍한 쓰레드들은 어떻게 하나,, 전능하신 운영체제께서 알아서..ㅎ 
	// 임마 쇼하지 말고 나중에...초반에 핸들값들 다 벡터에 차곡차곡 모아서 여기서 멈추게해드려..
	isSaveOn = true;
	if (!isSaveOn) {
		isOnSaveUserDataThread = false;
		printf(" [System] User Data Last Save Success! \n\n");
		CloseHandle(hSaveUserDataThread);
	}

}


//thread's
DWORD WINAPI IOCPServer::WorkerThread(LPVOID arg)
{
	IOCPServer* server = (IOCPServer*)arg;
	server->WorkerThreadFunction();

	return 0;
};

void IOCPServer::WorkerThreadFunction()
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
		SOCKETINFO *ptr;

		// 입출력 완료 포트에 저장된 결과를 처리하기 위한 함수 // 대기 상태가 됨
		retVal = GetQueuedCompletionStatus(
			hIOCP, //입출력 완료 포트 핸들
			&cbTransferred, //비동기 입출력 작업으로, 전송된 바이트 수가 여기에 저장된다.
			&clientSocket, //함수 호출 시 전달한 세번째 인자(32비트) 가 여기에 저장된다.
			(LPOVERLAPPED *)&ptr, //Overlapped 구조체의 주소값
			INFINITE // 대기 시간 -> 깨울떄 까지 무한대
		);
#pragma endregion
		//std::cout << "Thread Fire!!" << std::endl;
#pragma region [ Get Socket and error Exception ]

		// 할당받은 소켓 즉! 클라이언트 정보 얻기
		SOCKADDR_IN clientAddr;
		int addrLength = sizeof(clientAddr);
		getpeername(ptr->sock, (SOCKADDR *)&clientAddr, &addrLength);

		//비동기 입출력 결과 확인 // 아무것도 안보낼 때는, 해당 클라이언트 접속에 문제가 생긴것으로 판단, 닫아버리겠다!
		// 근데 이거 에코서버일떄만 그래야되는거 아니야???? 몰봐 임마 뭘봐 모를수도 있지
		if (retVal == 0 || cbTransferred == 0)
		{
			// 게임 방에서 나갔을 경우, 서버의 해당 클라의 ptr->RoomIndex = -1로 변경하는 작업 필요.
			if (ptr->roomIndex >= 0)
			{
				if (int ReturnEnemyIndexBuffer; roomData.SignOut(ptr->roomIndex, ptr->isHost, ReturnEnemyIndexBuffer))
				{
					// 게임 중이였을 경우, 현재 클라이언트의 패배 처리 및, 상대 클라이언트 승리 처리
					userData.SetGameResult(ReturnEnemyIndexBuffer, true);
					userData.SetGameResult(ptr->userIndex, false);
				}
				else
				{
					// 게임 중이 아니였거나, 완전히 다른 거 중. 
					//...? 할게 없나...?
				}
			}

			userData.SignOut(ptr->userIndex);

			//std::cout << "DEBUG - Error or Exit Client A" << std::endl;
			if (retVal == 0)
			{
				DWORD temp1, temp2;
				WSAGetOverlappedResult(ptr->sock, &ptr->overlapped, &temp1, FALSE, &temp2);
				NETWORK_UTIL::ERROR_DISPLAY((char *)"WSAGetOverlappedResult()");
			}
			closesocket(ptr->sock);

#ifdef _DEBUG
			std::cout << "     [Notify] 클라이언트 종료 혹은 연결 끊김 Id = " << userData.GetUserID(ptr->userIndex) << std::endl;
			//printf("[TCP 서버] 클라이언트 종료 : IP 주소 =%s, 포트 번호 =%d\n",
			//	inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
#endif
			delete ptr;
			continue;
		}

#pragma endregion

		if (ptr->bufferProtocol == END_SEND)
		{
			ptr->bufferProtocol = START_RECV;

			//std::cout << "ptr->bufferProtocol == -1" << std::endl;

			ptr->wsabuf.buf = ptr->buf;
			ptr->wsabuf.len = BUF_SIZE;

			retVal = WSARecv(clientSocket, &ptr->wsabuf, 1, &recvBytes, &flags, &ptr->overlapped, NULL);
			// 클라이언트 소켓, 읽을 데이터 버퍼의 포인터, 데이터 입력 버퍼의 개수, recv 결과 읽은 바이트 수, IOCP에서는 비동기 방식으로 사용하지 않으므로 nullPtr를 넘겨도 무방,  recv에 사용될 플래그,
			// overlapped구조체의 포인터, IOCP에서는 사용하지 않으므로 NULL, nullptr넘겨도 무방

			if (retVal == SOCKET_ERROR)
			{
				if (WSAGetLastError() != ERROR_IO_PENDING)
				{
					NETWORK_UTIL::ERROR_DISPLAY((char *)"WSARecv()");
				}
				continue;
			}

		}
		else if (ptr->isRecvTrue)
		{
			if (ptr->bufferProtocol == START_RECV)
			{
				recvType = (int&)(ptr->buf);

				std::cout << "UserIndex : " << ptr->userIndex << " ,  recvType == " << recvType << std::endl;

				/*
				if (recvType == DEMAND_GAMESTATE)
				{
				if (!selfControl)
				{
				int buffer = 400;
				memcpy(ptr->buf, (char*)&buffer, sizeof(int));

				ptr->dataSize = sizeof(int);

				ptr->wsabuf.buf = ptr->buf; // ptr->buf;
				ptr->wsabuf.len = ptr->dataSize;

				ptr->bufferProtocol = END_SEND;
				ptr->isRecvTrue = true;

				DWORD sendBytes;
				retVal = WSASend(ptr->sock, &ptr->wsabuf, 1, &sendBytes, 0, &ptr->overlapped, NULL);

				if (retVal == SOCKET_ERROR)
				{
				if (WSAGetLastError() != WSA_IO_PENDING)
				{
				err_display((char *)"WSASend()");
				}
				continue;
				}

				selfControl = 0;
				continue;
				}
				else if (selfControl == 1)
				{
				ptr->dataBuffer = new OnePlayerChanged(1, 1, 0);
				}
				else if (selfControl == 2)
				{
				ptr->dataBuffer = new OnePlayerChanged(1, 1, 1);
				}
				else if (selfControl == 3)
				{
				ptr->dataBuffer = new OnePlayerChanged(1, 2, 0);
				}
				else if (selfControl == 4)
				{
				ptr->dataBuffer = new OnePlayerChanged(1, 2, 1);
				}
				else if (selfControl == 5)
				{
				ptr->dataBuffer = new OnePlayerChanged(1, 0, 0);
				}
				else if (selfControl == 6)
				{
				ptr->dataBuffer = new OnePlayerChanged(1, 0, 1);
				}

				int buffer = 401;
				memcpy(ptr->buf, (char*)&buffer, sizeof(int));
				memcpy(ptr->buf + 4, (char*)ptr->dataBuffer, sizeof(OnePlayerChanged));

				ptr->dataSize = sizeof(int) + sizeof(OnePlayerChanged);

				//데이터 바인드
				ptr->wsabuf.buf = ptr->buf; // ptr->buf;
				ptr->wsabuf.len = ptr->dataSize;

				// 다음 통신 준비
				ptr->bufferProtocol = END_SEND;
				ptr->isRecvTrue = true;

				DWORD sendBytes;
				retVal = WSASend(ptr->sock, &ptr->wsabuf, 1, &sendBytes, 0, &ptr->overlapped, NULL);

				if (retVal == SOCKET_ERROR)
				{
				if (WSAGetLastError() != WSA_IO_PENDING)
				{
				err_display((char *)"WSASend()");
				}
				continue;
				}

				selfControl = 0;
				}
				*/

				// LoginScene
				if (recvType == DEMAND_LOGIN)
				{
					//DemandLoginCharStruct demandLogin = (DemandLoginCharStruct&)(ptr->buf[4]);
					//std::cout << "아이디 비밀번호, 타입을 입력 받았습니다. ID:  " << demandLogin.ID << "  PW : " << demandLogin.PW << "  type : " << demandLogin.type << std::endl;
					int typeBuffer = (int&)(ptr->buf[4]);
					int pwBuffer = (int&)(ptr->buf[8]);
					int idSizeBuffer = (int&)(ptr->buf[12]);
					string idBuffer;

					for (int i = 0; i < idSizeBuffer; ++i)
					{
						//idBuffer.append(&(ptr->buf[16+i]));
						idBuffer += ptr->buf[16 + i];
					}

					if (typeBuffer == 1) //로그인 처리입니다.
					{
						int outWinCount{};
						int outLoseCount{};
						int outMoney{};

						int failReason = userData.SignIn(idBuffer, pwBuffer, outWinCount, outLoseCount, outMoney, ptr->userIndex);

						if (!failReason) {
							//std::cout << "로그인에 성공했습니다. " << std::endl;

							// 데이터 준비
							ptr->dataBuffer = new PermitLoginStruct(outWinCount, outLoseCount, outMoney);
							//permitLoginStruct* a = static_cast<PermitLoginStruct *>(ptr->dataBuffer);

							if (NETWORK_UTIL::SendProcess(ptr, sizeof(int) + sizeof(PermitLoginStruct), PERMIT_LOGIN))
								continue;
						}
						else if (failReason) {
							//std::cout << "로그인에 실패했습니다.  해당 사유는 : " << failReason << std::endl;

							// 데이터 준비
							ptr->dataBuffer = new FailLoginStruct(failReason);

							if (NETWORK_UTIL::SendProcess(ptr, sizeof(int) + sizeof(FailLoginStruct), FAIL_LOGIN))
								continue;
						}
					}
					else if (typeBuffer == 2) //회원가입 처리입니다.
					{
						int failReason = userData.SignUp(idBuffer);

						if (!failReason) {
							//std::cout << "회원가입에 성공했습니다. " << std::endl;

							// 회원 가입 처리
							userData.EmplaceBackToPlayer(idBuffer, pwBuffer, ptr->userIndex);

							// 저장 쓰레드 호출
							isSaveOn = true;

							// 데이터 준비
							ptr->dataBuffer = new PermitLoginStruct(0, 0, 0);

							if (NETWORK_UTIL::SendProcess(ptr, sizeof(int) + sizeof(PermitLoginStruct), PERMIT_LOGIN))
								continue;
						}
						else if (failReason) {

							//std::cout << "회원가입에 실패했습니다.  해당 사유는 : " << failReason << std::endl;

							// 데이터 준비
							ptr->dataBuffer = new FailLoginStruct(failReason);

							if (NETWORK_UTIL::SendProcess(ptr, sizeof(int) + sizeof(FailLoginStruct), FAIL_LOGIN))
								continue;
						}
					}
				}

				//LobbyScene - old
				else if (recvType == DEMAND_MAKEROOM)
				{
					ptr->roomIndex = roomData.CreateRoom(ptr->userIndex);
					ptr->isHost = true;

					ptr->dataBuffer = new PermitMakeRoomStruct(ptr->roomIndex);

					if (NETWORK_UTIL::SendProcess(ptr, sizeof(int) + sizeof(PermitMakeRoomStruct), PERMIT_MAKEROOM))
						continue;
				}
				else if (recvType == DEMAND_JOINROOM)
				{
					int roomIndexBuffer = (int&)(ptr->buf[4]);

					int failReasonBuffer = roomData.JoinRoom(ptr->userIndex, roomIndexBuffer);

					if (failReasonBuffer)
					{
						//std::cout << roomIndexBuffer << " 방으로의 입장을 실패했습니다. 실패 사유 : " << failReasonBuffer << endl;

						ptr->dataBuffer = new FailJoinRoomStruct(failReasonBuffer);

						if (NETWORK_UTIL::SendProcess(ptr, sizeof(int) + sizeof(FailJoinRoomStruct), FAIL_JOINROOM))
							continue;
					}
					else
					{
						//std::cout << roomIndexBuffer << " 번째 방으로의 입장을 성공했습니다. 방장 아이디는 : (error) 비밀입니다. " << endl;

						ptr->isHost = false;
						ptr->roomIndex = roomIndexBuffer;

						ptr->dataBuffer = new PermitJoinRoomStruct(roomIndexBuffer, userData.GetUserID(roomData.GetEnemyIndex(ptr->roomIndex, ptr->isHost)));

						if (NETWORK_UTIL::SendProcess(ptr, sizeof(int) + sizeof(PermitJoinRoomStruct), PERMIT_JOINROOM))
							continue;
					}
				}

				//LobbyScene - new
				else if (recvType == DEMAND_RANDOM_MATCH)
				{
					if (roomData.RandomMatchingProcess(ptr->userIndex, ptr->roomIndex)) // Create!!
					{
						ptr->isHost = true;

						int retIsHostFirst, retPlayerMissionIndex, retEnemyMissionIndex, retSubMissionIndex;
						roomData.GetRoomGameData(ptr->roomIndex, ptr->isHost, retIsHostFirst, retPlayerMissionIndex, retEnemyMissionIndex, retSubMissionIndex);

						ptr->dataBuffer = new PermitMakeRandomStruct(ptr->roomIndex, retIsHostFirst, retPlayerMissionIndex, retEnemyMissionIndex, retSubMissionIndex);

						if (NETWORK_UTIL::SendProcess(ptr, sizeof(int) + sizeof(PermitMakeRandomStruct), PERMIT_MAKE_RANDOM))
							continue;
					}
					else // Join!
					{
						ptr->isHost = false;

						int retIsHostFirst, retPlayerMissionIndex, retEnemyMissionIndex, retSubMissionIndex;
						roomData.GetRoomGameData(ptr->roomIndex, ptr->isHost, retIsHostFirst, retPlayerMissionIndex, retEnemyMissionIndex, retSubMissionIndex);

						ptr->dataBuffer = new PermitJoinRandomStruct(ptr->roomIndex, retIsHostFirst, retPlayerMissionIndex, retEnemyMissionIndex, retSubMissionIndex, userData.GetUserID(roomData.GetEnemyIndex(ptr->roomIndex, ptr->isHost)));

						if (NETWORK_UTIL::SendProcess(ptr, sizeof(int) + sizeof(PermitJoinRandomStruct), PERMIT_JOIN_RANDOM))
							continue;
					}
				}
				else if (recvType == DEMAND_GUEST_JOIN)
				{
					if (roomData.GetGameReady(ptr->roomIndex))
					{
						ptr->dataBuffer = new PermitGuestJoinStruct(userData.GetUserID(roomData.GetEnemyIndex(ptr->roomIndex, ptr->isHost)));

						if (NETWORK_UTIL::SendProcess(ptr, sizeof(int) + sizeof(PermitGuestJoinStruct), PERMIT_GUEST_JOIN))
							continue;
					}
					else
					{
						if (NETWORK_UTIL::SendProcess(ptr, sizeof(int), PERMIT_GUEST_NOT_JOIN))
							continue;
					}
				}

				//RoomScene - old
				else if (recvType == DEMAND_ROOMHOST) {
					// 게스트가 들어왔을 때,
					if (roomData.GetGameReady(ptr->roomIndex))
					{
						ptr->dataBuffer = new RoomStateGuestInStruct(userData.GetUserID(roomData.GetEnemyIndex(ptr->roomIndex, ptr->isHost)));

						if (NETWORK_UTIL::SendProcess(ptr, sizeof(int) + sizeof(RoomStateGuestInStruct), ROOMSTATE_GUESTIN))
							continue;
					}
					// 아무도 안들어 왔을 때,
					else {
						if (NETWORK_UTIL::SendProcess(ptr, sizeof(int), ROOMSTATE_VOID))
							continue;
					}
				}

				//RoomScene - new
				else if (recvType == DEMAND_ENEMY_CHARACTER)
				{
					roomData.SetCharacterIndex(ptr->roomIndex, ptr->isHost, (int&)ptr->buf[4]);

					ptr->dataBuffer = new PermitEnemyCharacterStruct(roomData.GetEnemyCharacterIndex(ptr->roomIndex, ptr->isHost));

					if (NETWORK_UTIL::SendProcess(ptr, sizeof(int) + sizeof(PermitEnemyCharacterStruct), PERMIT_ENEMY_CHARACTER))
						continue;
				}

				// GameScene - Defense Turn
				else if (recvType == DEMAND_GAME_STATE)
				{
					// Caution!! Get Atomic!!
					int dataProtocol = roomData.GetDataProtocol(ptr->roomIndex, ptr->isHost);

					cout << "   debug 1 : dataProtocol - " << dataProtocol <<  "\n";

					if (dataProtocol == NOTIFY_CHANGE_TURN)
					{
						std::cout << "턴 변경 : 받는 쪽에서 확인 완료 \n";

						if (NETWORK_UTIL::SendProcess(ptr, sizeof(int), NOTIFY_CHANGE_TURN))
						{
							// Caution!! dataBuffer Delete
							//roomData.DeleteDataBuffer(ptr->roomIndex, ptr->isHost);
							// Caution!! Set Atomic!!
							roomData.SetDataProtocol(ptr->roomIndex, !(ptr->isHost), VOID_GAME_STATE);
							continue;
						}
					}
					else if (dataProtocol != VOID_GAME_STATE && dataProtocol != 0)	// 리팩토링
					{
						ptr->dataBuffer = roomData.GetDataBuffer(ptr->roomIndex, ptr->isHost);

						if (NETWORK_UTIL::SendProcess(ptr, sizeof(int) + sizeof(ptr->dataBuffer), dataProtocol))
						{
							// Caution!! dataBuffer Delete
							roomData.DeleteDataBuffer(ptr->roomIndex, ptr->isHost);
							// Caution!! Set Atomic!!
							roomData.SetDataProtocol(ptr->roomIndex, ptr->isHost, VOID_GAME_STATE);
							continue;
						}
					}
					else
					{
						if (NETWORK_UTIL::SendProcess(ptr, sizeof(int), VOID_GAME_STATE))
							continue;
					}
				}

				//GameScene - Attack Turn
				else if (recvType > 500 && recvType < 600)
				{
					// Caution!! Get Atomic!!
					//int dataProtocol = roomData.GetDataProtocol(ptr->roomIndex, ptr->isHost);
					//if (dataProtocol)
					//{
						// 지연된 확인 필요.
						//Error 
						// 너무나도 높은 확률로, 수비턴의 클라이언트 네트워크 상태가 불안정할 가능성이 큽니다.
						// 하아...이걸 어떻게 처리 한담...
					//}
					//else 
					//{
					if (recvType == NOTIFY_END_OF_TURN)
					{
						std::cout << "1 턴 변경 : 보내는 쪽에서 저장 완료 \n";
						roomData.SetDataProtocol(ptr->roomIndex, ptr->isHost, NOTIFY_CHANGE_TURN);
						
						std::cout << "1 너네 방 몇번이냐 ㅡㅡ" << ptr->roomIndex << "\n";

						roomData.GetDataProtocol(ptr->roomIndex, true);
						roomData.GetDataProtocol(ptr->roomIndex, false);

						std::cout << "2 너네 방 몇번이냐 ㅡㅡ" << ptr->roomIndex << "\n";
					}
					else if (recvType == VOID_CLIENT_TO_SERVER)
					{
						// Caution!! Set Atomic!!
						roomData.SetDataProtocol(ptr->roomIndex, ptr->isHost, VOID_SERVER_TO_CLIENT);
					}
					else if (recvType == CHANGE_PLANET_CLIENT_TO_SERVER)
					{
						//roomData.GetDataBuffer(ptr->roomIndex) = new ChangePlanetStruct((int&)(ptr->buf[4]), (int&)(ptr->buf[8]), (int*)ptr->buf[12]);
						roomData.SetDataBuffer(ptr->roomIndex, ptr->isHost, ChangePlanetStruct((int&)(ptr->buf[4]), (int&)(ptr->buf[8]), (int*)ptr->buf[12]));
						// Caution!! Set Atomic!!
						roomData.SetDataProtocol(ptr->roomIndex, ptr->isHost, CHANGE_PLANET_SERVER_TO_CLIENT);
					}
					else if (recvType == ACTION_EVENTCARD_TERRAIN_CLIENT_TO_SERVER)
					{
						roomData.SetDataBuffer(ptr->roomIndex, ptr->isHost, ActionEventCardTerrainStruct((int&)(ptr->buf[4]), (int&)(ptr->buf[8]), (int&)(ptr->buf[12]), (int*)ptr->buf[16]));
						// Caution!! Set Atomic!!
						roomData.SetDataProtocol(ptr->roomIndex, ptr->isHost, ACTION_EVENTCARD_TERRAIN_SERVER_TO_CLIENT);
					}
					else if (recvType == ACTION_EVENTCARD_DICEBUFF_CLIENT_TO_SERVER)
					{
						roomData.SetDataBuffer(ptr->roomIndex, ptr->isHost, ActionEventCardDiceBuffStruct((int&)(ptr->buf[4])));
						// Caution!! Set Atomic!!
						roomData.SetDataProtocol(ptr->roomIndex, ptr->isHost, ACTION_EVENTCARD_DICEBUFF_SERVER_TO_CLIENT);
					}

					// Send Process!!

					int dataProtocol = roomData.GetDataProtocol(ptr->roomIndex, ptr->isHost);

					if (dataProtocol != VOID_GAME_STATE && dataProtocol != 0)	// 리팩토링
					{
						std::cout << "\n" << ptr->userIndex << "가 ERROR!!!! 501이 아닌 다른 값을 보내려고 함 그럼 그 값은?? " << dataProtocol << "\n";

						ptr->dataBuffer = roomData.GetDataBuffer(ptr->roomIndex, ptr->isHost);

						if (NETWORK_UTIL::SendProcess(ptr, sizeof(int) + sizeof(ptr->dataBuffer), dataProtocol))
						{
							// Caution!! dataBuffer Delete
							roomData.DeleteDataBuffer(ptr->roomIndex, ptr->isHost);
							// Caution!! Set Atomic!!
							roomData.SetDataProtocol(ptr->roomIndex, ptr->isHost, VOID_GAME_STATE);
							continue;
						}
					}
					else
					{
						if (NETWORK_UTIL::SendProcess(ptr, sizeof(int), VOID_GAME_STATE))
							continue;
					}
				}

				// Network Exception - RoomScene 
				else if (recvType == DOUBLECHECK_DISCONNECTED_ENEMY_CLIENT)
				{
					// 이거 써야 좋으려나 안쓰는게 좋으려나. --> 써야것다. 변수 하나 씩 다 추가하는거 오바다.
					// 이 함수는, 상대편 클라이언트가 나갔을 경우, 이를 확인하고, 관련 클라이언트 처리가 끝났음을 알리는 프로토콜에 대한 처리입니다.
					roomData.ExitRoom(ptr->roomIndex);
					ptr->roomIndex = -1;
				}

				// Hey!! It is garbage!!!!! ERROR!!! OhMyGod!!!!!!! !!!! WT!!!
				else
				{
					//std::cout << "Not! Defined recvType Error  recvType == " << recvType << std::endl;
					//std::cout << "OMG!! this Thread Down!!" << std::endl;

#ifdef _DEBUG
					std::cout << "     [Error] Not! Defined recvType Error // Client Down! //  recvType == " << recvType << std::endl;
#endif
				}
			}
		}
		else if (!(ptr->isRecvTrue))
		{
		}
	}
}

DWORD WINAPI IOCPServer::SaveUserDate(LPVOID arg)
{
	IOCPServer* server = (IOCPServer*)arg;
	server->SaveUserDataThreadFunction();

	return 0;
}

void IOCPServer::SaveUserDataThreadFunction()
{
	isOnSaveUserDataThread = true;

	while (isOnSaveUserDataThread) {
		Sleep(1000);	// 1분 단위 저장

		//지금 자야되니까 나중에 이거보면 
		//isSaveOn == if문 함수 밖으로 뺴라 멍청아... 이걸 거따가 집어넣었네ㅡㅡ
		// ? 레퍼런스로 안에서 바꿔줄건데 바보?

		userData.Save(isSaveOn);
	}
}

//DWORD WINAPI IOCPServer::ServerTestThread(LPVOID arg)
//{
//	//while (7) {
//	//	Sleep(1000);
//	//	if (!selfControl) {
//	//		//selfControl = rand() % 6 + 1;
//	//		std::cout << "보낼 패킷 선택 // 1 왼쪽, 2 왼 점프, 3 오른, 4 오 점, 5 정지, 6 정 점프  : ";
//	//		std::cin >> selfControl;
//	//		//std::cout << "클라이언트에게 새로운 입력 패킷  : " << selfControl << "를 전달했습니다. " << std::endl;
//	//	}
//	//}
//}
