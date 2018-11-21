#pragma once

#include "../PCH/stdafx.h"
#include "../IOCPServer/Custom_Node_Queue.h"

class SocketInfo;

enum UDP_PROTOCOL 
{
		INVITE_FRIEND = 1
	,	DEMAND_FRIEND = 2
	,	DENY_FRIEND	=	3
};

class UDPManager
{
	//enum UDP_PROTOCOL
	//{
	//	DEMAND_FRIEND =	(char)1,
	//	//PERMIT_FRIEND =	(char)2,  // TCP
	//	//FAIL_FRIEND	 =	(char)3,  // TCP
	//	
	//	INVITE_ROOM = (char)2,
	//	//PERMIT_INVITE = (char)5, // TCP
	//	//FAIL_INVITE = (char)6	   // TCP
	//};

	const char INVITE_ROOM;
	const char DEMAND_FRIEND;
	const char DYNAMIC_MESSAGE;

	const USHORT UDP_PORT;

	SOCKET udpSocket;
	SOCKADDR_IN serverUDPAddr;

	Custom_Node_Queue friendInviteMessageQueue;
	Custom_Node_Queue friendDemandMessageQueue;

public:
	UDPManager();
	~UDPManager() = default;

public:
	__inline void Push(const int& InContNumber, SocketInfo* pInSocket)
	{
		switch (InContNumber)
		{
		case UDP_PROTOCOL::INVITE_FRIEND:
			friendInviteMessageQueue.Push(pInSocket);
			break;
		case UDP_PROTOCOL::DEMAND_FRIEND:
			friendDemandMessageQueue.Push(pInSocket);
			break;
		}
	}

	void UDPSend()
	{
		if (!friendInviteMessageQueue.IsEmpty())
		{
			_SendMessage(INVITE_ROOM);
		}
		else if (!friendDemandMessageQueue.IsEmpty())
		{
			_SendMessage(DEMAND_FRIEND);
		}
	}

public:
	void _CreateUDPSocket();

public:
	void _SendMessage(const char& InChar);
};