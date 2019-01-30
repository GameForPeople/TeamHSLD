#pragma once

#include "../PCH/stdafx.h"
#include "../Custom_DataStructure/Custom_Node_Queue.h"

class UserData;

enum UDP_PROTOCOL : int
{
		INVITE_FRIEND = 1
	,	DEMAND_FRIEND = 2

	,	RESULT_FRIEND =	7
	,	ANNOUNCEMENT = 8	// �ش� ������, ��ü �������� �����ؾ��ϹǷ�, ť������ �ƴ�, �ٸ��� ����.
};

class UDPManager
{
	const char				CONST_INVITE_FRIEND;
	const char				CONST_DEMAND_FRIEND;

	const char				CONST_RESULT_FRIEND;
	const char				CONST_ANNOUNCEMENT;

	const USHORT			UDP_PORT;

	SOCKET					udpSocket;
	SOCKADDR_IN				serverUDPAddr;

	CUSTOM_QUEUE::CustomQueue<weak_ptr<UserData>> friendInviteMessageQueue;	// INVITE_FRIEND = 1
	CUSTOM_QUEUE::CustomQueue<weak_ptr<UserData>> friendDemandMessageQueue; // DEMAND_FRIEND = 2

	CUSTOM_QUEUE::CustomQueue<weak_ptr<UserData>> friendResultMessageQueue;	// RESULT_FRIEND = 7

	//std::vector<CUSTOM_QUEUE::CustomQueue<weak_ptr<UserData>>> messageQueue;
	//CRITICAL_SECTION		csUDP;
	
	wstring announceString;

public:
	UDPManager() noexcept;
	~UDPManager();

public:
	__inline void Push(const int InContNumber, const shared_ptr<UserData>& pInUserData)
	{
		// �ణ �̰ŵ� ��ȿ����
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

	// UDP SEND ���ο��� while(7)���� ���鼭 �˻��Ұ���, Thread���� �ֵ������� �Ұ��� ���ؾ���
	__inline void UDPSend()
	{
		if (!friendInviteMessageQueue.IsEmpty())
		{
			_SendInviteMessage();
		}
		else if (!friendDemandMessageQueue.IsEmpty())
		{
			_SendDemandMessage();
		}
		else if (!friendResultMessageQueue.IsEmpty())
		{
			_SendResultMessage();
		}
	}

public:
	void _CreateUDPSocket();

public:
	//void _SendMessage(const char InChar);
	void _SendInviteMessage();
	void _SendDemandMessage();
	void _SendResultMessage();

public:
	void _SendAnnouncement(const shared_ptr<UserData>&);
	_NODISCARD __inline wstring& GetAnnounceString() noexcept { return announceString; };	// ���۷��� �����.
	__inline void SetAnnounceString(const wstring_view& InAnnounceString) noexcept { announceString = InAnnounceString; };
};