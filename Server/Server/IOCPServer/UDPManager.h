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

	,	ANNOUNCEMENT = 8	// �ش� ������, ��ü �������� �����ؾ��ϹǷ�, ť������ �ƴ�, �ٸ��� ����.

	,	SEND_PORT = 9
	,	CONFIRM_PORT = 10

	,	DUMMY_FOR_STUN = 11
};

// UDP Manager ������ ������ �ʿ���...
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
		// �ణ �̰ŵ� ��ȿ����
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

	// UDP SEND ���ο��� while(7)���� ���鼭 �˻��Ұ���, Thread���� �ֵ������� �Ұ��� ���ؾ���
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
	_NODISCARD __inline wstring& GetAnnounceString() noexcept { return announceString; };	// ���۷��� �����.
	__inline void SetAnnounceString(const wstring_view& InAnnounceString) noexcept { announceString = InAnnounceString; };
};