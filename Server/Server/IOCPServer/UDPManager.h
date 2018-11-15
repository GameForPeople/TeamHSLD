#pragma once

#include "../PCH/stdafx.h"
#include "../IOCPServer/Custom_Node_Queue.h"

class SocketInfo;

class UDPManager
{
	SOCKET udpSocket;
	SOCKADDR_IN serverUDPAddr;

	Custom_Node_Queue friendMessageQueue;

public:
	UDPManager() = default;
	~UDPManager() = default;

public:
	__inline void Push(int InContNumber, SocketInfo* pInSocket, const char& InChar)
	{
		switch (InContNumber)
		{
		case 1:
			friendMessageQueue.Push(make_pair(pInSocket, InChar));
		}
	}

	void UDPSend()
	{
		if (!friendMessageQueue.IsEmpty())
		{
			_SendFriendMessage();
		}
	}

public:
	void _CreateUDPSocket();

public:
	void _SendFriendMessage();

};