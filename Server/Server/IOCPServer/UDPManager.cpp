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

UDPManager::UDPManager() 
	: CONST_INVITE_FRIEND(static_cast<char>(1))
	, CONST_DEMAND_FRIEND(static_cast<char>(2))
	, CONST_RESULT_FRIEND(static_cast<char>(7))
	, UDP_PORT(htons(SERVER_UDP_PORT))
{}

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

void UDPManager::_SendMessage(const char& InChar)
{
	weak_ptr<UserData> pNodeBuffer; // = friendInviteMessageQueue.Pop();
	
	// 이부분 비효율적; 차라리. 함수 여러개 만들기.
	if (InChar == CONST_INVITE_FRIEND)
		pNodeBuffer = friendInviteMessageQueue.Pop();
	else if (InChar == CONST_DEMAND_FRIEND)
		pNodeBuffer = friendDemandMessageQueue.Pop();

	else if (InChar == CONST_RESULT_FRIEND)
		pNodeBuffer = friendResultMessageQueue.Pop();
	else return;

	SOCKADDR_IN clientAddr;
	int addrLength = sizeof(clientAddr);

	shared_ptr<UserData> pUserData = pNodeBuffer.lock();

	if (pUserData == nullptr) // 댕글링 포인터 에러. 이미 딤진 소켓. 여기서 이거 nullptr 절대 안걸려 지고 고냥 서버 터짐.
	{
		std::cout << "Error UDP Send \n";
		return;
	}

	getpeername(pUserData->GetSocketInfo()->sock, reinterpret_cast<SOCKADDR *>(&clientAddr), &addrLength);
	clientAddr.sin_port = UDP_PORT;

	if (int retValue 
		= sendto(udpSocket, reinterpret_cast<const char*>(&InChar), 1, 0, reinterpret_cast<SOCKADDR*>(&clientAddr), sizeof(clientAddr))
		; retValue == SOCKET_ERROR)
	{
		UDP_UTIL::ERROR_QUIT((char*)"UDP_SEND_ERROR()");
	}

	//std::cout << " UDP -> " << (int)(InChar) << "를 보냈습니다.  \n";

	//delete pUserData;
	//delete pNodeBuffer;
}
