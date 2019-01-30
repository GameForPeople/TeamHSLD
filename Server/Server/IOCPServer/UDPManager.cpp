#include "../IOCPServer/UDPManager.h"
#include "../IOCPServer/SocketInfo.h"

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
	: CONST_INVITE_FRIEND(static_cast<char>(1))
	, CONST_DEMAND_FRIEND(static_cast<char>(2))
	, CONST_RESULT_FRIEND(static_cast<char>(7))
	, CONST_ANNOUNCEMENT(static_cast<char>(8))
	, UDP_PORT(htons(SERVER_UDP_PORT))
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

void UDPManager::_SendInviteMessage()
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
	clientAddr.sin_port = UDP_PORT;

	//reinterpret_cast<const char*>(&CONST_INVITE_FRIEND)
	if (int retValue
		= sendto(udpSocket, &CONST_INVITE_FRIEND, 1, 0, reinterpret_cast<SOCKADDR*>(&clientAddr), sizeof(clientAddr))
		; retValue == SOCKET_ERROR)
	{
		UDP_UTIL::ERROR_QUIT((char*)"UDP_SEND_ERROR()");
	}

	//std::cout << "[UDP_Manager] UDP Message "<< CONST_INVITE_FRIEND << "를 " << pUserData->GetNickname() <<"에게 전송했습니다. \n";
}

void UDPManager::_SendDemandMessage()
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
	clientAddr.sin_port = UDP_PORT;

	if (int retValue
		= sendto(udpSocket, reinterpret_cast<const char*>(&CONST_DEMAND_FRIEND), 1, 0, reinterpret_cast<SOCKADDR*>(&clientAddr), sizeof(clientAddr))
		; retValue == SOCKET_ERROR)
	{
		UDP_UTIL::ERROR_QUIT((char*)"UDP_SEND_ERROR()");
	}
}

void UDPManager::_SendResultMessage()
{
	SOCKADDR_IN clientAddr;
	int addrLength = sizeof(clientAddr);

	//weak_ptr<UserData> pNodeBuffer = friendResultMessageQueue.Pop();
	shared_ptr<UserData> pUserData = friendResultMessageQueue.Pop().lock();

	if (pUserData == nullptr) // 댕글링 포인터 제어. 이미 딤진 소켓.
	{
#ifdef _DEBUG_MODE_
		std::cout << "[UDP_Manager] 이미 로그아웃한 계정입니다.\n";
#endif // _DEBUG_MODE_
		return;
	}

	getpeername(pUserData->GetSocketInfo()->sock, reinterpret_cast<SOCKADDR *>(&clientAddr), &addrLength);
	clientAddr.sin_port = UDP_PORT;

	if (int retValue
		= sendto(udpSocket, reinterpret_cast<const char*>(&CONST_RESULT_FRIEND), 1, 0, reinterpret_cast<SOCKADDR*>(&clientAddr), sizeof(clientAddr))
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
	clientAddr.sin_port = UDP_PORT;

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
