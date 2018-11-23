#pragma once

#include "../PCH/stdafx.h"
#include "../IOCPServer/Custom_Node_Queue.h"

class SocketInfo;

enum UDP_PROTOCOL 
{
		INVITE_FRIEND = 1
	,	DEMAND_FRIEND = 2
	,	RESULT_FRIEND =	7
};

class UDPManager
{
	const char CONST_INVITE_FRIEND;
	const char CONST_DEMAND_FRIEND;
	const char CONST_RESULT_FRIEND;

	const USHORT UDP_PORT;

	SOCKET udpSocket;
	SOCKADDR_IN serverUDPAddr;

	Custom_Node_Queue friendInviteMessageQueue;	// INVITE_FRIEND = 1
	Custom_Node_Queue friendDemandMessageQueue; // DEMAND_FRIEND = 2

	Custom_Node_Queue friendResultMessageQueue;	// RESULT_FRIEND = 7
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
		case UDP_PROTOCOL::RESULT_FRIEND:	
			friendResultMessageQueue.Push(pInSocket);
			break;
		}
	}

	void UDPSend()
	{
		if (!friendInviteMessageQueue.IsEmpty())
		{
			_SendMessage(CONST_INVITE_FRIEND);
		}
		else if (!friendDemandMessageQueue.IsEmpty())
		{
			_SendMessage(CONST_DEMAND_FRIEND);
		}



		else if (!friendResultMessageQueue.IsEmpty())
		{
			_SendMessage(CONST_RESULT_FRIEND);
		}
	}

public:
	void _CreateUDPSocket();

public:
	void _SendMessage(const char& InChar);
};