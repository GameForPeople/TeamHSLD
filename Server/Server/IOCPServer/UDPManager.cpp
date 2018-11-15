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
		UDP_UTIL::ERROR_QUIT((char *)"bind()");
}

void UDPManager::_SendFriendMessage()
{
	CustomNode* pNodeBuffer = friendMessageQueue.Pop();

	SOCKADDR_IN clientAddr;
	int addrLength = sizeof(clientAddr);

	if (pNodeBuffer->GetData().first != nullptr) 
	{
		getpeername(pNodeBuffer->GetData().first->sock, (SOCKADDR *)&clientAddr, &addrLength);
		clientAddr.sin_port = htons(SERVER_UDP_PORT);

		sendto(udpSocket, (char*)(pNodeBuffer->GetData().second), 1, 0, (sockaddr*)&clientAddr, sizeof(clientAddr));
	}
	delete pNodeBuffer;
}