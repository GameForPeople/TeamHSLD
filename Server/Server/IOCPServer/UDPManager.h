#pragma once

#include "../PCH/stdafx.h"
#include "../Custom_DataStructure/Custom_Node_Queue.h"

class UserData;
class UserDataManager;

enum UDP_PROTOCOL : int
{
		INVITE_FRIEND = 1
	,	DEMAND_FRIEND = 2

	,	RESULT_TRUE_FRIEND = 6
	,   RESULT_FAIL_FRIEND = 7

	,	ANNOUNCEMENT = 8	// 해당 사항은, 전체 유저에게 전송해야하므로, 큐형식이 아닌, 다르게 구현.

	,	SEND_PORT = 9
	,	CONFIRM_PORT = 10

	,	DUMMY_FOR_STUN = 11
};

// UDP Manager 구조의 변경이 필요함...
class UDPManager
{
	const char				CONST_INVITE_FRIEND;
	const char				CONST_DEMAND_FRIEND;

	const char				CONST_RESULT_TRUE_FRIEND;
	const char				CONST_RESULT_FAIL_FRIEND;

	const char				CONST_ANNOUNCEMENT;
	const char				CONST_CONFIRM_PORT;

	const USHORT			UDP_PORT;

	SOCKET					udpSocket;
	SOCKADDR_IN				serverUDPAddr;

	CUSTOM_QUEUE::CustomQueue<weak_ptr<UserData>> friendInviteMessageQueue;	// INVITE_FRIEND = 1
	CUSTOM_QUEUE::CustomQueue<weak_ptr<UserData>> friendDemandMessageQueue; // DEMAND_FRIEND = 2

	CUSTOM_QUEUE::CustomQueue<weak_ptr<UserData>> friendTrueResultMessageQueue;	// RESULT_FRIEND = 6
	CUSTOM_QUEUE::CustomQueue<weak_ptr<UserData>> friendFailResultMessageQueue;	// RESULT_FRIEND = 6

	CUSTOM_QUEUE::CustomQueue<weak_ptr<UserData>> confirmPortMessageQueue; // CONFIRM_PORT = 10
	//std::vector<CUSTOM_QUEUE::CustomQueue<weak_ptr<UserData>>> messageQueue;
	//CRITICAL_SECTION		csUDP;
	
	wstring announceString;

public:
	UDPManager() noexcept;
	~UDPManager();

public:
	__inline void Push(const int InContNumber, const shared_ptr<UserData>& pInUserData)
	{
		// 약간 이거도 비효율적
		switch (InContNumber)
		{
		case UDP_PROTOCOL::INVITE_FRIEND:
			friendInviteMessageQueue.Push(pInUserData);
			break;
		case UDP_PROTOCOL::DEMAND_FRIEND:
			friendDemandMessageQueue.Push(pInUserData);
			break;
		case UDP_PROTOCOL::RESULT_TRUE_FRIEND:
			friendTrueResultMessageQueue.Push(pInUserData);
			break;
		case UDP_PROTOCOL::RESULT_FAIL_FRIEND:
			friendFailResultMessageQueue.Push(pInUserData);
			break;
		case UDP_PROTOCOL::CONFIRM_PORT:
			confirmPortMessageQueue.Push(pInUserData);
			break;
		}
	}

	// UDP SEND 내부에서 while(7)문을 돌면서 검사할건지, Thread에서 주도적으로 할건지 정해야함
	__inline void UDPSend()
	{
		while (7)
		{
			if (!friendInviteMessageQueue.IsEmpty())
			{
				_SendFriendInviteMessage();
			}
			else if (!friendDemandMessageQueue.IsEmpty())
			{
				_SendFriendDemandMessage();
			}
			
			else if (!friendTrueResultMessageQueue.IsEmpty())
			{
				_SendFriendTrueResultMessage();
			}
			else if (!friendFailResultMessageQueue.IsEmpty())
			{
				_SendFriendFailResultMessage();
			}
			else if (!confirmPortMessageQueue.IsEmpty())
			{
				_SendConfirmMessage();
			}

			Sleep(1000);
		}
	}

	void UDPRecv(UserDataManager* InUserDataManager);

public:
	void _CreateUDPSocket();

public:
	//void _SendMessage(const char InChar);
	void _SendFriendInviteMessage();
	void _SendFriendDemandMessage();
	void _SendFriendTrueResultMessage();
	void _SendFriendFailResultMessage();
	void _SendConfirmMessage();

public:
	void _SendAnnouncement(const shared_ptr<UserData>& InSafeUserDataPtr);
	_NODISCARD __inline wstring& GetAnnounceString() noexcept { return announceString; };	// 레퍼런스 보장됨.
	__inline void SetAnnounceString(const wstring_view& InAnnounceString) noexcept { announceString = InAnnounceString; };
};