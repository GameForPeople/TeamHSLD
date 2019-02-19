#include "../IOCPServer/UDPManager.h"
#include "../IOCPServer/SocketInfo.h"

#include "../UserData/UserData.h"
#include "../UserData/UserDataManager.h"

namespace UDP_UTIL {
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
};

UDPManager::UDPManager() noexcept
	: CONST_INVITE_FRIEND(static_cast<char>(UDP_PROTOCOL::INVITE_FRIEND))
	, CONST_DEMAND_FRIEND(static_cast<char>(UDP_PROTOCOL::DEMAND_FRIEND))
	, CONST_RESULT_TRUE_FRIEND(static_cast<char>(UDP_PROTOCOL::RESULT_TRUE_FRIEND))
	, CONST_RESULT_FAIL_FRIEND(static_cast<char>(UDP_PROTOCOL::RESULT_FAIL_FRIEND))
	, CONST_ANNOUNCEMENT(static_cast<char>(UDP_PROTOCOL::ANNOUNCEMENT))
	, CONST_CONFIRM_PORT(static_cast<char>(UDP_PROTOCOL::CONFIRM_PORT))
	, UDP_PORT(htons(CLIENT_UDP_PORT))
	, udpSocket()
	, serverUDPAddr()
	//, csUDP()
{
	//::InitializeCriticalSection(&csUDP);
}

UDPManager::~UDPManager()
{
	//::DeleteCriticalSection(&csUDP);
}

void UDPManager::_CreateUDPSocket()
{
	udpSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if (udpSocket == INVALID_SOCKET) UDP_UTIL::ERROR_QUIT((char *)"Socket()");

	ZeroMemory(&serverUDPAddr, sizeof(serverUDPAddr));
	serverUDPAddr.sin_family = AF_INET;
	serverUDPAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverUDPAddr.sin_port = htons(SERVER_UDP_PORT);

	if (int retval = ::bind(udpSocket, (SOCKADDR *)&serverUDPAddr, sizeof(serverUDPAddr));
	retval == SOCKET_ERROR)
		UDP_UTIL::ERROR_QUIT((char *)"UDP_bind()");
}

void UDPManager::UDPRecv(UserDataManager* InUserDataManager)
{
	// 어쩔수 없이, 함수 내부에서, 계속 생성하도록 합니다...

	SOCKADDR_IN clientAddr;
	char recvBuffer[50];
	int addrlen(sizeof(clientAddr));

	int retVal = recvfrom(udpSocket, recvBuffer, 50, 0, (SOCKADDR *)&clientAddr, &addrlen);
	if (retVal == SOCKET_ERROR)
	{
#ifdef _DEBUG_MODE_
		std::cout << "[UDPManager] UDP_Receive에서 Error가 발생했습니다. \n";
#endif
	}
	
	if (recvBuffer[0] == UDP_PROTOCOL::SEND_PORT)
	{
		int idLength = recvBuffer[1];

		if (idLength < 20)
		{
			recvBuffer[2 + idLength] = '\0';
			Type_ID idString(recvBuffer + 2);

			//std::cout << "UDP로 받은 메세지는 : " << idString << std::endl;

			bool isLogin(false);

			if (auto pUserNode = InUserDataManager->SearchUserNode(idString, isLogin)
				; isLogin == true)
			{
				pUserNode->SetUdpPortNumber(/*ntohs*/(clientAddr.sin_port));

				//std::cout << "받은 ip 주소는 : " << inet_ntoa(clientAddr.sin_addr) << std::endl;
				//std::cout << "받은 포트 번호 는 : " << ntohs(clientAddr.sin_port) << std::endl;

				// UDP 메세지 적재.
				//Push(UDP_PROTOCOL::CONFIRM_PORT, pUserNode);

				if (int retValue
					= sendto(udpSocket, reinterpret_cast<const char*>(&CONST_CONFIRM_PORT), 1, 0, reinterpret_cast<SOCKADDR*>(&clientAddr), sizeof(clientAddr))
					; retValue == SOCKET_ERROR)
				{
					UDP_UTIL::ERROR_QUIT((char*)"UDP_SEND_ERROR()");
				}
			}
			else
			{
#ifdef _DEBUG_MODE_
				std::cout << "[UDP_Manager] 해당 유저가 나갔다면 OK, 안나갔다면...? ㅎ 에러야.... \n";
				// UDP Packet이 손실돼 아이디가 이상해졌다면?? <- 사실 그럴 수 없음.
#endif // _DEBUG_MODE_
			}
		}
		else 
		{
#ifdef _DEBUG_MODE_
			std::cout << "[UDP_Manager] 아이디 Size가 20글자를 넘을리가 없어요.... UDP Packet이 나쁜놈들에게 당했습니다.";
#endif // _DEBUG_MODE_
		}
	}
	else if (recvBuffer[0] == UDP_PROTOCOL::DUMMY_FOR_STUN)
	{
		// Recv Only!
	}
}

void UDPManager::_SendFriendInviteMessage()
{
	SOCKADDR_IN clientAddr;
	int addrLength = sizeof(clientAddr);

	friendInviteMessageQueue.PrintHeapPointUseCount(); // --> 2

	//weak_ptr<UserData> pNodeBuffer = friendInviteMessageQueue.Pop();
	shared_ptr<UserData> pUserData = friendInviteMessageQueue.Pop().lock();

	if (pUserData == nullptr) // 댕글링 포인터 제어. 이미 딤진 소켓.
	{
#ifdef _DEBUG_MODE_
		std::cout << "[UDP_Manager] 이미 로그아웃한 계정입니다.\n";
#endif // _DEBUG_MODE_
		return;
	}

	getpeername(pUserData->GetSocketInfo()->sock, reinterpret_cast<SOCKADDR *>(&clientAddr), &addrLength);
	clientAddr.sin_port = pUserData->GetUdpPortNumber();

	//reinterpret_cast<const char*>(&CONST_INVITE_FRIEND)
	if (int retValue
		= sendto(udpSocket, &CONST_INVITE_FRIEND, 1, 0, reinterpret_cast<SOCKADDR*>(&clientAddr), sizeof(clientAddr))
		; retValue == SOCKET_ERROR)
	{
		UDP_UTIL::ERROR_QUIT((char*)"UDP_SEND_ERROR()");
	}

	//std::cout << "[UDP_Manager] UDP Message "<< CONST_INVITE_FRIEND << "를 " << pUserData->GetNickname() <<"에게 전송했습니다. \n";
}

void UDPManager::_SendFriendDemandMessage()
{
	SOCKADDR_IN clientAddr;
	int addrLength = sizeof(clientAddr);

	//weak_ptr<UserData> pNodeBuffer = friendDemandMessageQueue.Pop();
	shared_ptr<UserData> pUserData = friendDemandMessageQueue.Pop().lock();

	if (pUserData == nullptr) // 댕글링 포인터 제어. 이미 딤진 소켓.
	{
#ifdef _DEBUG_MODE_
		std::cout << "[UDP_Manager] 이미 로그아웃한 계정입니다.\n";
#endif // DEBUG
		return;
	}

	getpeername(pUserData->GetSocketInfo()->sock, reinterpret_cast<SOCKADDR *>(&clientAddr), &addrLength);
	clientAddr.sin_port = pUserData->GetUdpPortNumber();

	if (int retValue
		= sendto(udpSocket, reinterpret_cast<const char*>(&CONST_DEMAND_FRIEND), 1, 0, reinterpret_cast<SOCKADDR*>(&clientAddr), sizeof(clientAddr))
		; retValue == SOCKET_ERROR)
	{
		UDP_UTIL::ERROR_QUIT((char*)"UDP_SEND_ERROR()");
	}
}

void UDPManager::_SendFriendTrueResultMessage()
{
	SOCKADDR_IN clientAddr;
	int addrLength = sizeof(clientAddr);

	//weak_ptr<UserData> pNodeBuffer = friendResultMessageQueue.Pop();
	shared_ptr<UserData> pUserData = friendTrueResultMessageQueue.Pop().lock();

	if (pUserData == nullptr) // 댕글링 포인터 제어. 이미 딤진 소켓.
	{
#ifdef _DEBUG_MODE_
		std::cout << "[UDP_Manager] 이미 로그아웃한 계정입니다.\n";
#endif // _DEBUG_MODE_
		return;
	}

	getpeername(pUserData->GetSocketInfo()->sock, reinterpret_cast<SOCKADDR *>(&clientAddr), &addrLength);
	clientAddr.sin_port = pUserData->GetUdpPortNumber();

	if (int retValue
		= sendto(udpSocket, reinterpret_cast<const char*>(&CONST_RESULT_TRUE_FRIEND), 1, 0, reinterpret_cast<SOCKADDR*>(&clientAddr), sizeof(clientAddr))
		; retValue == SOCKET_ERROR)
	{
		UDP_UTIL::ERROR_QUIT((char*)"UDP_SEND_ERROR()");
	}
}

void UDPManager::_SendFriendFailResultMessage()
{
	SOCKADDR_IN clientAddr;
	int addrLength = sizeof(clientAddr);

	//weak_ptr<UserData> pNodeBuffer = friendResultMessageQueue.Pop();
	shared_ptr<UserData> pUserData = friendFailResultMessageQueue.Pop().lock();

	if (pUserData == nullptr) // 댕글링 포인터 제어. 이미 딤진 소켓.
	{
#ifdef _DEBUG_MODE_
		std::cout << "[UDP_Manager] 이미 로그아웃한 계정입니다.\n";
#endif // _DEBUG_MODE_
		return;
	}

	getpeername(pUserData->GetSocketInfo()->sock, reinterpret_cast<SOCKADDR *>(&clientAddr), &addrLength);
	clientAddr.sin_port = pUserData->GetUdpPortNumber();

	if (int retValue
		= sendto(udpSocket, reinterpret_cast<const char*>(&CONST_RESULT_FAIL_FRIEND), 1, 0, reinterpret_cast<SOCKADDR*>(&clientAddr), sizeof(clientAddr))
		; retValue == SOCKET_ERROR)
	{
		UDP_UTIL::ERROR_QUIT((char*)"UDP_SEND_ERROR()");
	}
}

void UDPManager::_SendConfirmMessage()
{
	SOCKADDR_IN clientAddr;
	int addrLength = sizeof(clientAddr);

	shared_ptr<UserData> pUserData = confirmPortMessageQueue.Pop().lock();

	if (pUserData == nullptr) // 댕글링 포인터 제어. 이미 딤진 소켓.
	{
#ifdef _DEBUG_MODE_
		std::cout << "[UDP_Manager] 이미 로그아웃한 계정입니다.\n";
#endif // _DEBUG_MODE_
		return;
	}

	getpeername(pUserData->GetSocketInfo()->sock, reinterpret_cast<SOCKADDR *>(&clientAddr), &addrLength);
	clientAddr.sin_port = /*htons*/(pUserData->GetUdpPortNumber());

	std::cout << "보내는 ip 주소는 : " << inet_ntoa(clientAddr.sin_addr) << std::endl;
	std::cout << "보내는 포트 번호 는 : " << htons(clientAddr.sin_port) << std::endl;

	if (int retValue
		= sendto(udpSocket, reinterpret_cast<const char*>(&CONST_CONFIRM_PORT), 1, 0, reinterpret_cast<SOCKADDR*>(&clientAddr), sizeof(clientAddr))
		; retValue == SOCKET_ERROR)
	{
		UDP_UTIL::ERROR_QUIT((char*)"UDP_SEND_ERROR()");
	}
}

void UDPManager::_SendAnnouncement(const shared_ptr<UserData>& pInUserData)
{
	SOCKADDR_IN clientAddr;
	int addrLength = sizeof(clientAddr);

	shared_ptr<UserData> pUserData = pInUserData;

	if (pUserData == nullptr) // 댕글링 포인터 제어. 이미 딤진 소켓.
	{
		return;
	}

	getpeername(pUserData->GetSocketInfo()->sock, reinterpret_cast<SOCKADDR *>(&clientAddr), &addrLength);
	
	//std::cout << " 받는 계정의 IP는 : " << inet_ntoa(clientAddr.sin_addr);

	clientAddr.sin_port = /*htons*/(pUserData->GetUdpPortNumber());

	if (int retValue
		= sendto(udpSocket, reinterpret_cast<const char*>(&CONST_ANNOUNCEMENT), 1, 0, reinterpret_cast<SOCKADDR*>(&clientAddr), sizeof(clientAddr))
		; retValue == SOCKET_ERROR)
	{
		UDP_UTIL::ERROR_QUIT((char*)"UDP_SEND_ERROR()");
	}
}

/*
void UDPManager::_SendMessage(const char InChar)
{
	std::cout << "[DEBUG_UDP] 0 \n";
	weak_ptr<UserData> pNodeBuffer; // = friendInviteMessageQueue.Pop();
	std::cout << "[DEBUG_UDP] 1 \n";

	// 이부분 비효율적; 차라리. 함수 여러개 만들기.
	if (InChar == CONST_INVITE_FRIEND)
	{
		pNodeBuffer = friendInviteMessageQueue.Pop();
		std::cout << "[DEBUG_UDP] 1 - A \n";
	}
	else if (InChar == CONST_DEMAND_FRIEND)
	{
		pNodeBuffer = friendDemandMessageQueue.Pop();
		std::cout << "[DEBUG_UDP] 1 - B \n";
	}
	else if (InChar == CONST_RESULT_FRIEND)
	{
		pNodeBuffer = friendResultMessageQueue.Pop();
		std::cout << "[DEBUG_UDP] 1 - C \n";
	}
	else return;

	std::cout << "[DEBUG_UDP] 2 \n";

	SOCKADDR_IN clientAddr;
	int addrLength = sizeof(clientAddr);

	shared_ptr<UserData> pUserData = pNodeBuffer.lock();

	std::cout << "[DEBUG_UDP] 3 \n";

	if (pUserData == nullptr) // 댕글링 포인터 에러. 이미 딤진 소켓. 여기서 이거 nullptr 절대 안걸려 지고 고냥 서버 터짐.
	{
		std::cout << "Error UDP Send \n";
		return;
	}

	std::cout << "[DEBUG_UDP] 4 \n";

	getpeername(pUserData->GetSocketInfo()->sock, reinterpret_cast<SOCKADDR *>(&clientAddr), &addrLength);
	clientAddr.sin_port = UDP_PORT;

	std::cout << "[DEBUG_UDP] 5 \n";

	if (int retValue 
		= sendto(udpSocket, reinterpret_cast<const char*>(&InChar), 1, 0, reinterpret_cast<SOCKADDR*>(&clientAddr), sizeof(clientAddr))
		; retValue == SOCKET_ERROR)
	{
		UDP_UTIL::ERROR_QUIT((char*)"UDP_SEND_ERROR()");
	}

	std::cout << "[DEBUG_UDP] 6 \n";

	//std::cout << " UDP -> " << (int)(InChar) << "를 보냈습니다.  \n";

	//delete pUserData;
	//delete pNodeBuffer;
}
*/
