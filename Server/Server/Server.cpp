#include "stdafx.h"
#include "Server.h"

#include <atomic>

#define SERVER_DEBUG_LOG_PRINT

static CUserData userData;	//쓰레드에서도 사용해야하는데, 유저 데이터가 겁나많으면 어떻게 하려고!!, 전역으로 선언해서 걍 쓰자!ㅎㅎㅎ
static bool isSaveOn{ false };	// 저장 여부 판단 (클라에 의해, 정보 변경 요청 받을 시 true로 변경 / 굳이 동기화 필요없이 sleep로 처리/ 나중에 바이너리 적용되면 시간 크게 고려 ㄴ)
static int SelfControl{ 0 };

DWORD WINAPI WorkerThread(LPVOID arg);
DWORD WINAPI SaveUserDate(LPVOID arg);
DWORD WINAPI ServerTestThread(LPVOID arg);

int main(int argc, char * argv[])
{
#pragma region [Server UI]
	char* retIPChar;
	retIPChar = new char[20]; // IPv4가 20 char보다 클일 죽어도 없음.
	GetExternalIP(retIPChar);

	printf("■■■■■■■■■■■■■■■■■■■■■■■■■\n");
	printf("■ IOCP Server  - Bridge Unity Project          \n");
	printf("■                                ver 0.1 180815\n");
	printf("■\n");
	printf("■    IP Address : %s \n", retIPChar);
	printf("■    Server Port : %d \n", SERVER_PORT);
	printf("■■■■■■■■■■■■■■■■■■■■■■■■■\n\n");

	delete[]retIPChar;
#pragma endregion

#pragma region [Load UserData]
	userData.Load();
#pragma endregion

#pragma region [ 윈속 초기화 및 입출력 완료 포트 생성 ]
	//윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return 1;

	// 입출력 완료 포트 생성
	HANDLE hcp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	/*
	CreateIoCompletionPort는 두가지 역할을 함!
	1. 입출력 완료 포트 생성
	2. 소켓과 입출력 완료 포트 연결 (IO장치와 IOCP연결)

	1번째 인자값,  IOCP와 연결할 핸들, 생성시는 INVALID_HANDLE_VALUE를 인자로 넘김
	2번째 인자값,  IOCP 핸들, 첫 생성시는 NULL
	3번째 인자값, IO완료시 넘어갈 값, 사용자가 넘기고 싶은 값 넘김
	4번째 인자값, 한번에 동작할 최대 스레드 개수, 0 넘기면 프로세스 숫자로 자동 지정됨
	*/

	if (hcp == NULL)
	{
		printf("입출력 완료 포트 생성 실패");
		return 1;
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
		hThread = CreateThread(NULL, 0, WorkerThread, hcp, 0, NULL);
		if (hThread == NULL) return 1;
		CloseHandle(hThread);
	}

#pragma endregion

#pragma region [ 소켓 생성 및, 바인드, 리슨 ]

	//Socket()
	SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (listenSocket == INVALID_SOCKET) err_quit((char *)"socket()");

	//bind()
	SOCKADDR_IN serverAddr;
	ZeroMemory(&serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = htons(SERVER_PORT);
	int retVal = bind(listenSocket, (SOCKADDR *)&serverAddr, sizeof(serverAddr));
	if (retVal == SOCKET_ERROR) err_quit((char *)"bind()");

	// Listen()!
	retVal = listen(listenSocket, SOMAXCONN);
	if (retVal == SOCKET_ERROR) err_quit((char *)"listen()");

#pragma endregion

#pragma region [ Thread Run! Accept and Save UserData ]

	printf(" [System] Dedicated server activated!\n\n");

	SOCKET clientSocket;
	SOCKADDR_IN clientAddr;
	int addrLength;
	DWORD recvBytes, flags;

	HANDLE hSaveUserDataThread;
	hSaveUserDataThread = CreateThread(NULL, 0, SaveUserDate, NULL, 0, NULL);
	CloseHandle(hSaveUserDataThread);

	//HANDLE hServerTestThread;
	//hServerTestThread = CreateThread(NULL, 0, ServerTestThread, NULL, 0, NULL);
	//CloseHandle(hSaveUserDataThread);

	while (7) {
		//accept()
		addrLength = sizeof(clientAddr);
		clientSocket = accept(listenSocket, (SOCKADDR *)&clientAddr, &addrLength);
		if (clientSocket == INVALID_SOCKET)
		{
			err_display((char *)"accept()");
			break;
		}

		// 클라이언트 서버에 접속(Accept) 함을 알림
		printf("[TCP 서버] 클라이언트 접속 : IP 주소 =%s, Port 번호 = %d \n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

		// 소켓과 입출력 완료 포트 연결
		CreateIoCompletionPort((HANDLE)clientSocket, hcp, clientSocket, 0);

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
		retVal = WSARecv(
			clientSocket, // 클라이언트 소켓
			&ptr->wsabuf, // 읽을 데이터 버퍼의 포인터
			1,			 // 데이터 입력 버퍼의 개수
			&recvBytes,  // recv 결과 읽은 바이트 수, IOCP에서는 비동기 방식으로 사용하지 않으므로 nullPtr를 넘겨도 무방
			&flags,		 // recv에 사용될 플래그
			&ptr->overlapped, // overlapped구조체의 포인터
			NULL			// IOCP에서는 사용하지 않으므로 NULL, nullptr넘겨도 무방
		);

		if (retVal == SOCKET_ERROR)
		{
			if (WSAGetLastError() != ERROR_IO_PENDING)
			{
				err_display((char *)"WSARecv()");
			}

			continue;
		}
	}
#pragma endregion

#pragma region [ Destroy() && plzDoNotQuit! ]
	char plzDoNotQuit{};
	std::cin >> plzDoNotQuit;

	closesocket(listenSocket);
	WSACleanup();

	//....? 우리 열심히 일한 불쌍한 쓰레드들은 어떻게 하나,, 전능하신 운영체제께서 알아서..ㅎ 
	// 임마 쇼하지 말고 나중에...초반에 핸들값들 다 벡터에 차곡차곡 모아서 여기서 멈추게해드려..
	return 0;
#pragma endregion
}

DWORD WINAPI WorkerThread(LPVOID arg)
{
	HANDLE hcp = (HANDLE)arg;

	int retVal{};
	int recvType{};

	while (7)
	{
		//std::cout << "W";
#pragma region [ Wait For Thread ]
		//비동기 입출력 기다리기
		DWORD cbTransferred;
		SOCKET clientSocket;
		SOCKETINFO *ptr;

		// 입출력 완료 포트에 저장된 결과를 처리하기 위한 함수 // 대기 상태가 됨
		retVal = GetQueuedCompletionStatus(
			hcp, //입출력 완료 포트 핸들
			&cbTransferred, //비동기 입출력 작업으로, 전송된 바이트 수가 여기에 저장된다.
			&clientSocket, //함수 호출 시 전달한 세번째 인자(32비트) 가 여기에 저장된다.
			(LPOVERLAPPED *)&ptr, //Overlapped 구조체의 주소값
			INFINITE // 대기 시간 -> 깨울떄 까지 무한대
		);
#pragma endregion
		//std::cout << "F" << std::endl;
#pragma region [ Get Socket and error Exception ]

		// 할당받은 소켓 즉! 클라이언트 정보 얻기
		SOCKADDR_IN clientAddr;
		int addrLength = sizeof(clientAddr);
		getpeername(ptr->sock, (SOCKADDR *)&clientAddr, &addrLength);

		//비동기 입출력 결과 확인 // 아무것도 안보낼 때는, 해당 클라이언트 접속에 문제가 생긴것으로 판단, 닫아버리겠다!
		// 근데 이거 에코서버일떄만 그래야되는거 아니야???? 몰봐 임마 뭘봐 모를수도 있지
		if (retVal == 0 || cbTransferred == 0)
		{
			std::cout << "DEBUG - Error or Exit Client A" << std::endl;

			if (retVal == 0)
			{
				DWORD temp1, temp2;
				WSAGetOverlappedResult(ptr->sock, &ptr->overlapped, &temp1, FALSE, &temp2);
				err_display((char *)"WSAGetOverlappedResult()");
			}
			closesocket(ptr->sock);

			printf("[TCP 서버] 클라이언트 종료 : IP 주소 =%s, 포트 번호 =%d\n",
				inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
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

			// 비동기 입출력의 시작
			DWORD flags = 0;
			DWORD recvBytes{};
			 
			retVal = WSARecv(clientSocket, &ptr->wsabuf, 1, &recvBytes, &flags, &ptr->overlapped, NULL); 
			// 클라이언트 소켓, 읽을 데이터 버퍼의 포인터, 데이터 입력 버퍼의 개수, recv 결과 읽은 바이트 수, IOCP에서는 비동기 방식으로 사용하지 않으므로 nullPtr를 넘겨도 무방,  recv에 사용될 플래그,
			// overlapped구조체의 포인터, IOCP에서는 사용하지 않으므로 NULL, nullptr넘겨도 무방

			if (retVal == SOCKET_ERROR)
			{
				if (WSAGetLastError() != ERROR_IO_PENDING)
				{
					err_display((char *)"WSARecv()");
				}
				continue;
			}
		}
		else if (ptr->isRecvTrue)
		{
			if (ptr->bufferProtocol == START_RECV) 
			{
				recvType = (int&)(ptr->buf);
				//std::cout << "대기중이던 쓰레드가 받은 요구 데이터는 recvType == " << recvType << std::endl;

				if (recvType == DEMAND_LOGIN) 
				{
					DemandLoginCharStruct demandLogin = (DemandLoginCharStruct&)(ptr->buf[4]);
					demandLogin.ID[demandLogin.IDSize] = '\0';
					std::cout << "아이디 비밀번호, 타입을 입력 받았습니다. ID:  " << demandLogin.ID << "  PW : " << demandLogin.PW << "  type : " << demandLogin.type << std::endl;
					
					if (demandLogin.type == 1) //로그인 처리입니다.
					{
						int outWinCount{};
						int outLoseCount{};
						int outMoney{};

						int failReason = userData.SignIn(demandLogin.ID, demandLogin.PW, outWinCount, outLoseCount, outMoney);

						if (!failReason) {
							std::cout << "로그인에 성공했습니다. " << std::endl;

							// 데이터 준비
							ptr->dataBuffer = new PermitLoginStruct(outWinCount, outLoseCount, outMoney);
							//permitLoginStruct* a = static_cast<PermitLoginStruct *>(ptr->dataBuffer);
							ZeroMemory(&ptr->overlapped, sizeof(ptr->overlapped));
							
							int buffer = PERMIT_LOGIN;
							memcpy(ptr->buf, (char*)&buffer, sizeof(int));
							memcpy(ptr->buf + 4, (char*)ptr->dataBuffer, sizeof(PermitLoginStruct));
							
							ptr->dataSize = sizeof(int) + sizeof(PermitLoginStruct);

							//데이터 바인드
							ptr->wsabuf.buf = ptr->buf; // ptr->buf;
							ptr->wsabuf.len = ptr->dataSize;

							// 다음 통신 준비
							ptr->bufferProtocol = END_SEND;
							ptr->isRecvTrue = true;
							
							DWORD sendBytes;
							retVal = WSASend( ptr->sock, &ptr->wsabuf, 1, &sendBytes, 0, &ptr->overlapped,NULL);

							if (retVal == SOCKET_ERROR)
							{
								if (WSAGetLastError() != WSA_IO_PENDING)
								{
									err_display((char *)"WSASend()");
								}
								continue;
							}
						}
						else if (failReason) {
							std::cout << "로그인에 실패했습니다.  해당 사유는 : " << failReason << std::endl;

							// 데이터 준비
							ZeroMemory(&ptr->overlapped, sizeof(ptr->overlapped));

							int buffer = FAIL_LOGIN;
							memcpy(ptr->buf, (char*)(&buffer), sizeof(buffer));
							memcpy(ptr->buf + 4, (char*)(&failReason), sizeof(int));

							ptr->dataSize = 8; // 4+ 4

							// 데이터 바인딩
							ptr->wsabuf.buf = ptr->buf; // ptr->buf;
							ptr->wsabuf.len = ptr->dataSize;

							// 다음 통신 준비
							ptr->bufferProtocol = END_SEND;
							ptr->isRecvTrue = true;
							
							// 데이터 전송
							DWORD sendBytes;
							retVal = WSASend( ptr->sock, &ptr->wsabuf, 1, &sendBytes, 0, &ptr->overlapped, NULL);
							if (retVal == SOCKET_ERROR)
							{
								if (WSAGetLastError() != WSA_IO_PENDING)
								{
									err_display((char *)"WSASend()");
								}
								continue;
							}

							/*
							// 데이터 준비
							ptr->dataBuffer = new BaseSendStruct(failReason, (new FailLoginStruct(failReason)));
							ZeroMemory(&ptr->overlapped, sizeof(ptr->overlapped));

							int buffer = FAIL_LOGIN;
							memcpy(ptr->buf, (char*)&buffer, sizeof(buffer));
							ptr->dataSize = sizeof(buffer);

							// 데이터 바인딩
							ptr->wsabuf.buf = ptr->buf; // ptr->buf;
							ptr->wsabuf.len = ptr->dataSize;

							// 다음 통신 준비
							ptr->bufferProtocol = FAIL_LOGIN;
							ptr->isRecvTrue = false;

							// 데이터 전송
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
							*/
						}
					}
					else if (demandLogin.type == 2) //회원가입 처리입니다.
					{
						int failReason = userData.SignUp(demandLogin.ID);

						if (!failReason) {
							std::cout << "회원가입에 성공했습니다. " << std::endl;
							
							// 회원 가입 처리
							userData.EmplaceBackToPlayer(demandLogin.ID, demandLogin.PW);
							
							// 저장 쓰레드 호출
							isSaveOn = true;

							// 데이터 준비
							ZeroMemory(&ptr->overlapped, sizeof(ptr->overlapped));
							int buffer = PERMIT_LOGIN;
							memcpy(ptr->buf, (char*)&buffer, sizeof(buffer));
							buffer = 0; // 새로 회원 가입 시, 승리, 패배, 돈은 항상 ㅇ임 // 굳이 안보내도 되지만 일단 보내겠음 돈 줄수도 있으니까!
							//잊지마 4등록
							memcpy(ptr->buf + 4, (char*)&buffer, sizeof(buffer));
							memcpy(ptr->buf + 8, (char*)&buffer, sizeof(buffer));
							memcpy(ptr->buf + 12, (char*)&buffer, sizeof(buffer));

							ptr->dataSize = 16;

							// 데이터 바인딩
							ptr->wsabuf.buf = ptr->buf; // ptr->buf;
							ptr->wsabuf.len = ptr->dataSize;

							// 다음 데이터 준비
							ptr->bufferProtocol = END_SEND;
							ptr->isRecvTrue = true;

							// 데이터 전송
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
						}
						else if (failReason) {

							std::cout << "회원가입에 실패했습니다.  해당 사유는 : " << failReason << std::endl;

							// 데이터 준비
							ZeroMemory(&ptr->overlapped, sizeof(ptr->overlapped));
							int buffer = FAIL_LOGIN;
							memcpy(ptr->buf, (char*)&buffer, sizeof(buffer));
							memcpy(ptr->buf + 4, (char*)&failReason, sizeof(buffer));

							ptr->dataSize = 8;
							
							// 데이터 바인딩
							ptr->wsabuf.buf = ptr->buf; // ptr->buf;
							ptr->wsabuf.len = ptr->dataSize;

							// 다음 전송 데이터 준비
							ptr->bufferProtocol = END_SEND;
							ptr->isRecvTrue = true;

							// 데이터 전송
							DWORD sendBytes;
							retVal = WSASend( ptr->sock, &ptr->wsabuf, 1, &sendBytes, 0, &ptr->overlapped, NULL);
							if (retVal == SOCKET_ERROR)
							{
								if (WSAGetLastError() != WSA_IO_PENDING)
								{
									err_display((char *)"WSASend()");
								}
								continue;
							}
						}
					}
				}
				else if (recvType == DEMAND_GAMESTATE) 
				{
					if (!SelfControl)
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

						SelfControl = 0;
						continue;
					}
					else if (SelfControl == 1)
					{
						ptr->dataBuffer = new OnePlayerChanged(1, 1, 0);
					}
					else if (SelfControl == 2)
					{
						ptr->dataBuffer = new OnePlayerChanged(1, 1, 1);
					}
					else if (SelfControl == 3)
					{
						ptr->dataBuffer = new OnePlayerChanged(1, 2, 0);
					}
					else if (SelfControl == 4)
					{
						ptr->dataBuffer = new OnePlayerChanged(1, 2, 1);
					}
					else if (SelfControl == 5)
					{
						ptr->dataBuffer = new OnePlayerChanged(1, 0, 0);
					}
					else if (SelfControl == 6)
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

					SelfControl = 0;
				}
				else
				{
					std::cout << "Not! Defined recvType Error  recvType == "<< recvType << std::endl;
					std::cout << "OMG!! this Thread Down!!" << std::endl;
				}
			}
		}
		else if (!(ptr->isRecvTrue))
		{
		}
	}
};

DWORD WINAPI SaveUserDate(LPVOID arg) {
	while (7) {
		Sleep(10000);

		//지금 자야되니까 나중에 이거보면 isSaveOn == if문 함수 밖으로 뺴라 멍청아... 이걸 거따가 집어넣네
		userData.Save(isSaveOn);
	}
}

DWORD WINAPI ServerTestThread(LPVOID arg)
{
	while (7) {
		Sleep(1000);
		if (!SelfControl) {
			//SelfControl = rand() % 6 + 1;
			std::cout << "보낼 패킷 선택 // 1 왼쪽, 2 왼 점프, 3 오른, 4 오 점, 5 정지, 6 정 점프  : ";
			std::cin >> SelfControl;
			//std::cout << "클라이언트에게 새로운 입력 패킷  : " << SelfControl << "를 전달했습니다. " << std::endl;
		}
	}
}
