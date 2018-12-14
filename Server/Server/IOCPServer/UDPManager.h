#pragma once

#include "../PCH/stdafx.h"
#include "../Custom_DataStructure/Custom_Node_Queue.h"

class UserData;

enum UDP_PROTOCOL 
{
		INVITE_FRIEND = 1
	,	DEMAND_FRIEND = 2

	,	RESULT_FRIEND =	7
};

class UDPManager
{
	const char				CONST_INVITE_FRIEND;
	const char				CONST_DEMAND_FRIEND;

	const char				CONST_RESULT_FRIEND;

	const USHORT			UDP_PORT;

	SOCKET					udpSocket;
	SOCKADDR_IN				serverUDPAddr;

	CUSTOM_QUEUE::CustomQueue<weak_ptr<UserData>> friendInviteMessageQueue;	// INVITE_FRIEND = 1
	CUSTOM_QUEUE::CustomQueue<weak_ptr<UserData>> friendDemandMessageQueue; // DEMAND_FRIEND = 2

	CUSTOM_QUEUE::CustomQueue<weak_ptr<UserData>> friendResultMessageQueue;	// RESULT_FRIEND = 7

	//CRITICAL_SECTION		csUDP;

public:
	UDPManager() noexcept;
	~UDPManager();

public:
	__inline void Push(const int& InContNumber, const shared_ptr<UserData>& pInUserData)
	{
		switch (InContNumber)
		{
		case UDP_PROTOCOL::INVITE_FRIEND:
			friendInviteMessageQueue.Push(pInUserData);
			break;
		case UDP_PROTOCOL::DEMAND_FRIEND:
			friendDemandMessageQueue.Push(pInUserData);
			break;

		case UDP_PROTOCOL::RESULT_FRIEND:	
			friendResultMessageQueue.Push(pInUserData);
			break;
		}
	}

	__inline void UDPSend()
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