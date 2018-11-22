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
	: INVITE_ROOM(static_cast<char>(1)), DEMAND_FRIEND(static_cast<char>(2)), DYNAMIC_MESSAGE(static_cast<char>(3))
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
	CustomNode* pNodeBuffer; // = friendInviteMessageQueue.Pop();
	
	if(InChar == INVITE_ROOM)
		pNodeBuffer = friendInviteMessageQueue.Pop();
	else if (InChar == DEMAND_FRIEND)
		pNodeBuffer = friendDemandMessageQueue.Pop();

	SOCKADDR_IN clientAddr;
	int addrLength = sizeof(clientAddr);

	if (pNodeBuffer == nullptr)
	{
		std::cout << "Error UDP Send \n";
		return;
	}

	if (pNodeBuffer->GetData() != nullptr)
	{
		getpeername(pNodeBuffer->GetSocket(), (SOCKADDR *)&clientAddr, &addrLength);
		//clientAddr.sin_port = htons(SERVER_UDP_PORT);
		clientAddr.sin_port = UDP_PORT;

		if (int retValue = 
			sendto(udpSocket, reinterpret_cast<const char*>(&InChar), 1, 0, (sockaddr*)&clientAddr, sizeof(clientAddr))
			; retValue == SOCKET_ERROR)
		{
			UDP_UTIL::ERROR_QUIT((char*)"UDP_SEND_ERROR()");
		}
		//std::cout << "[UDP_MANAGER] 정상적으로 메세지 " << (int)InChar << "를 "<< pNodeBuffer->GetData()->pUserNode->SetValue().GetID() <<"보냈습니다. \n";
	}

	//delete pNodeBuffer;
	//pNodeBuffer = nullptr;
}
