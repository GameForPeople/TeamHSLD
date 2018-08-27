#include "stdafx.h"
#include "Server.h"

#include <atomic>

#define SERVER_DEBUG_LOG_PRINT

static CUserData userData;	//�����忡���� ����ؾ��ϴµ�, ���� �����Ͱ� �̳������� ��� �Ϸ���!!, �������� �����ؼ� �� ����!������
static bool isSaveOn{ false };	// ���� ���� �Ǵ� (Ŭ�� ����, ���� ���� ��û ���� �� true�� ���� / ���� ����ȭ �ʿ���� sleep�� ó��/ ���߿� ���̳ʸ� ����Ǹ� �ð� ũ�� ��� ��)
static int SelfControl{ 0 };

DWORD WINAPI WorkerThread(LPVOID arg);
DWORD WINAPI SaveUserDate(LPVOID arg);
DWORD WINAPI ServerTestThread(LPVOID arg);

int main(int argc, char * argv[])
{
#pragma region [Server UI]
	char* retIPChar;
	retIPChar = new char[20]; // IPv4�� 20 char���� Ŭ�� �׾ ����.
	GetExternalIP(retIPChar);

	printf("��������������������������\n");
	printf("�� IOCP Server  - Bridge Unity Project          \n");
	printf("��                                ver 0.1 180815\n");
	printf("��\n");
	printf("��    IP Address : %s \n", retIPChar);
	printf("��    Server Port : %d \n", SERVER_PORT);
	printf("��������������������������\n\n");

	delete[]retIPChar;
#pragma endregion

#pragma region [Load UserData]
	userData.Load();
#pragma endregion

#pragma region [ ���� �ʱ�ȭ �� ����� �Ϸ� ��Ʈ ���� ]
	//���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return 1;

	// ����� �Ϸ� ��Ʈ ����
	HANDLE hcp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	/*
	CreateIoCompletionPort�� �ΰ��� ������ ��!
	1. ����� �Ϸ� ��Ʈ ����
	2. ���ϰ� ����� �Ϸ� ��Ʈ ���� (IO��ġ�� IOCP����)

	1��° ���ڰ�,  IOCP�� ������ �ڵ�, �����ô� INVALID_HANDLE_VALUE�� ���ڷ� �ѱ�
	2��° ���ڰ�,  IOCP �ڵ�, ù �����ô� NULL
	3��° ���ڰ�, IO�Ϸ�� �Ѿ ��, ����ڰ� �ѱ�� ���� �� �ѱ�
	4��° ���ڰ�, �ѹ��� ������ �ִ� ������ ����, 0 �ѱ�� ���μ��� ���ڷ� �ڵ� ������
	*/

	if (hcp == NULL)
	{
		printf("����� �Ϸ� ��Ʈ ���� ����");
		return 1;
	}

	// CPU ���� Ȯ��
	SYSTEM_INFO si;
	GetSystemInfo(&si);

	// ���� �� CPU ���� == 12�� * 2���� �۾��� ������ ����
	// IO�۾��� �Ϸ�� ��, �Ϸ�� IO�� ���� ó���� ������ ������ Ǯ�� �����Ѵ�.
	// �Ϲ������� ������ Ǯ�� ũ��� ���μ��� ������ 2�� ������ �Ҵ��Ѵ�.
	
	// ������ ���� �� 2���� ���� ���� ���� �������� ���� ������ ������ �� �� �ִٰ� ������ �� �ִ� ���ϱ�..
	HANDLE hThread;
	for (int i = 0; i < (int)si.dwNumberOfProcessors * 2; ++i)
	{
		hThread = CreateThread(NULL, 0, WorkerThread, hcp, 0, NULL);
		if (hThread == NULL) return 1;
		CloseHandle(hThread);
	}

#pragma endregion

#pragma region [ ���� ���� ��, ���ε�, ���� ]

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

		// Ŭ���̾�Ʈ ������ ����(Accept) ���� �˸�
		printf("[TCP ����] Ŭ���̾�Ʈ ���� : IP �ּ� =%s, Port ��ȣ = %d \n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

		// ���ϰ� ����� �Ϸ� ��Ʈ ����
		CreateIoCompletionPort((HANDLE)clientSocket, hcp, clientSocket, 0);

		// ���� ���� ����ü �Ҵ�
		SOCKETINFO *ptr = new SOCKETINFO;
		if (ptr == NULL) break;

		ZeroMemory(&ptr->overlapped, sizeof(ptr->overlapped));

		ptr->sock = clientSocket;
		ptr->isRecvTrue = true;
		ptr->bufferProtocol = 0;

		ptr->wsabuf.buf = ptr->buf;
		ptr->wsabuf.len = BUF_SIZE;

		// �񵿱� ������� ����
		flags = 0;
		retVal = WSARecv(
			clientSocket, // Ŭ���̾�Ʈ ����
			&ptr->wsabuf, // ���� ������ ������ ������
			1,			 // ������ �Է� ������ ����
			&recvBytes,  // recv ��� ���� ����Ʈ ��, IOCP������ �񵿱� ������� ������� �����Ƿ� nullPtr�� �Ѱܵ� ����
			&flags,		 // recv�� ���� �÷���
			&ptr->overlapped, // overlapped����ü�� ������
			NULL			// IOCP������ ������� �����Ƿ� NULL, nullptr�Ѱܵ� ����
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

	//....? �츮 ������ ���� �ҽ��� ��������� ��� �ϳ�,, �����Ͻ� �ü������ �˾Ƽ�..�� 
	// �Ӹ� ������ ���� ���߿�...�ʹݿ� �ڵ鰪�� �� ���Ϳ� �������� ��Ƽ� ���⼭ ���߰��ص��..
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
		//�񵿱� ����� ��ٸ���
		DWORD cbTransferred;
		SOCKET clientSocket;
		SOCKETINFO *ptr;

		// ����� �Ϸ� ��Ʈ�� ����� ����� ó���ϱ� ���� �Լ� // ��� ���°� ��
		retVal = GetQueuedCompletionStatus(
			hcp, //����� �Ϸ� ��Ʈ �ڵ�
			&cbTransferred, //�񵿱� ����� �۾�����, ���۵� ����Ʈ ���� ���⿡ ����ȴ�.
			&clientSocket, //�Լ� ȣ�� �� ������ ����° ����(32��Ʈ) �� ���⿡ ����ȴ�.
			(LPOVERLAPPED *)&ptr, //Overlapped ����ü�� �ּҰ�
			INFINITE // ��� �ð� -> ��� ���� ���Ѵ�
		);
#pragma endregion
		//std::cout << "F" << std::endl;
#pragma region [ Get Socket and error Exception ]

		// �Ҵ���� ���� ��! Ŭ���̾�Ʈ ���� ���
		SOCKADDR_IN clientAddr;
		int addrLength = sizeof(clientAddr);
		getpeername(ptr->sock, (SOCKADDR *)&clientAddr, &addrLength);

		//�񵿱� ����� ��� Ȯ�� // �ƹ��͵� �Ⱥ��� ����, �ش� Ŭ���̾�Ʈ ���ӿ� ������ ��������� �Ǵ�, �ݾƹ����ڴ�!
		// �ٵ� �̰� ���ڼ����ϋ��� �׷��ߵǴ°� �ƴϾ�???? ���� �Ӹ� ���� �𸦼��� ����
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

			printf("[TCP ����] Ŭ���̾�Ʈ ���� : IP �ּ� =%s, ��Ʈ ��ȣ =%d\n",
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

			// �񵿱� ������� ����
			DWORD flags = 0;
			DWORD recvBytes{};
			 
			retVal = WSARecv(clientSocket, &ptr->wsabuf, 1, &recvBytes, &flags, &ptr->overlapped, NULL); 
			// Ŭ���̾�Ʈ ����, ���� ������ ������ ������, ������ �Է� ������ ����, recv ��� ���� ����Ʈ ��, IOCP������ �񵿱� ������� ������� �����Ƿ� nullPtr�� �Ѱܵ� ����,  recv�� ���� �÷���,
			// overlapped����ü�� ������, IOCP������ ������� �����Ƿ� NULL, nullptr�Ѱܵ� ����

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
				//std::cout << "������̴� �����尡 ���� �䱸 �����ʹ� recvType == " << recvType << std::endl;

				if (recvType == DEMAND_LOGIN) 
				{
					DemandLoginCharStruct demandLogin = (DemandLoginCharStruct&)(ptr->buf[4]);
					demandLogin.ID[demandLogin.IDSize] = '\0';
					std::cout << "���̵� ��й�ȣ, Ÿ���� �Է� �޾ҽ��ϴ�. ID:  " << demandLogin.ID << "  PW : " << demandLogin.PW << "  type : " << demandLogin.type << std::endl;
					
					if (demandLogin.type == 1) //�α��� ó���Դϴ�.
					{
						int outWinCount{};
						int outLoseCount{};
						int outMoney{};

						int failReason = userData.SignIn(demandLogin.ID, demandLogin.PW, outWinCount, outLoseCount, outMoney);

						if (!failReason) {
							std::cout << "�α��ο� �����߽��ϴ�. " << std::endl;

							// ������ �غ�
							ptr->dataBuffer = new PermitLoginStruct(outWinCount, outLoseCount, outMoney);
							//permitLoginStruct* a = static_cast<PermitLoginStruct *>(ptr->dataBuffer);
							ZeroMemory(&ptr->overlapped, sizeof(ptr->overlapped));
							
							int buffer = PERMIT_LOGIN;
							memcpy(ptr->buf, (char*)&buffer, sizeof(int));
							memcpy(ptr->buf + 4, (char*)ptr->dataBuffer, sizeof(PermitLoginStruct));
							
							ptr->dataSize = sizeof(int) + sizeof(PermitLoginStruct);

							//������ ���ε�
							ptr->wsabuf.buf = ptr->buf; // ptr->buf;
							ptr->wsabuf.len = ptr->dataSize;

							// ���� ��� �غ�
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
							std::cout << "�α��ο� �����߽��ϴ�.  �ش� ������ : " << failReason << std::endl;

							// ������ �غ�
							ZeroMemory(&ptr->overlapped, sizeof(ptr->overlapped));

							int buffer = FAIL_LOGIN;
							memcpy(ptr->buf, (char*)(&buffer), sizeof(buffer));
							memcpy(ptr->buf + 4, (char*)(&failReason), sizeof(int));

							ptr->dataSize = 8; // 4+ 4

							// ������ ���ε�
							ptr->wsabuf.buf = ptr->buf; // ptr->buf;
							ptr->wsabuf.len = ptr->dataSize;

							// ���� ��� �غ�
							ptr->bufferProtocol = END_SEND;
							ptr->isRecvTrue = true;
							
							// ������ ����
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
							// ������ �غ�
							ptr->dataBuffer = new BaseSendStruct(failReason, (new FailLoginStruct(failReason)));
							ZeroMemory(&ptr->overlapped, sizeof(ptr->overlapped));

							int buffer = FAIL_LOGIN;
							memcpy(ptr->buf, (char*)&buffer, sizeof(buffer));
							ptr->dataSize = sizeof(buffer);

							// ������ ���ε�
							ptr->wsabuf.buf = ptr->buf; // ptr->buf;
							ptr->wsabuf.len = ptr->dataSize;

							// ���� ��� �غ�
							ptr->bufferProtocol = FAIL_LOGIN;
							ptr->isRecvTrue = false;

							// ������ ����
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
					else if (demandLogin.type == 2) //ȸ������ ó���Դϴ�.
					{
						int failReason = userData.SignUp(demandLogin.ID);

						if (!failReason) {
							std::cout << "ȸ�����Կ� �����߽��ϴ�. " << std::endl;
							
							// ȸ�� ���� ó��
							userData.EmplaceBackToPlayer(demandLogin.ID, demandLogin.PW);
							
							// ���� ������ ȣ��
							isSaveOn = true;

							// ������ �غ�
							ZeroMemory(&ptr->overlapped, sizeof(ptr->overlapped));
							int buffer = PERMIT_LOGIN;
							memcpy(ptr->buf, (char*)&buffer, sizeof(buffer));
							buffer = 0; // ���� ȸ�� ���� ��, �¸�, �й�, ���� �׻� ���� // ���� �Ⱥ����� ������ �ϴ� �������� �� �ټ��� �����ϱ�!
							//������ 4���
							memcpy(ptr->buf + 4, (char*)&buffer, sizeof(buffer));
							memcpy(ptr->buf + 8, (char*)&buffer, sizeof(buffer));
							memcpy(ptr->buf + 12, (char*)&buffer, sizeof(buffer));

							ptr->dataSize = 16;

							// ������ ���ε�
							ptr->wsabuf.buf = ptr->buf; // ptr->buf;
							ptr->wsabuf.len = ptr->dataSize;

							// ���� ������ �غ�
							ptr->bufferProtocol = END_SEND;
							ptr->isRecvTrue = true;

							// ������ ����
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

							std::cout << "ȸ�����Կ� �����߽��ϴ�.  �ش� ������ : " << failReason << std::endl;

							// ������ �غ�
							ZeroMemory(&ptr->overlapped, sizeof(ptr->overlapped));
							int buffer = FAIL_LOGIN;
							memcpy(ptr->buf, (char*)&buffer, sizeof(buffer));
							memcpy(ptr->buf + 4, (char*)&failReason, sizeof(buffer));

							ptr->dataSize = 8;
							
							// ������ ���ε�
							ptr->wsabuf.buf = ptr->buf; // ptr->buf;
							ptr->wsabuf.len = ptr->dataSize;

							// ���� ���� ������ �غ�
							ptr->bufferProtocol = END_SEND;
							ptr->isRecvTrue = true;

							// ������ ����
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

					//������ ���ε�
					ptr->wsabuf.buf = ptr->buf; // ptr->buf;
					ptr->wsabuf.len = ptr->dataSize;

					// ���� ��� �غ�
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

		//���� �ھߵǴϱ� ���߿� �̰ź��� isSaveOn == if�� �Լ� ������ ���� ��û��... �̰� �ŵ��� ����ֳ�
		userData.Save(isSaveOn);
	}
}

DWORD WINAPI ServerTestThread(LPVOID arg)
{
	while (7) {
		Sleep(1000);
		if (!SelfControl) {
			//SelfControl = rand() % 6 + 1;
			std::cout << "���� ��Ŷ ���� // 1 ����, 2 �� ����, 3 ����, 4 �� ��, 5 ����, 6 �� ����  : ";
			std::cin >> SelfControl;
			//std::cout << "Ŭ���̾�Ʈ���� ���ο� �Է� ��Ŷ  : " << SelfControl << "�� �����߽��ϴ�. " << std::endl;
		}
	}
}
