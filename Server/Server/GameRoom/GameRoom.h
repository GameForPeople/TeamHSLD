#pragma once

#include "../PCH/stdafx.h"
#include "../Protocol/CommunicationDataStruct.h"
#include "../UserData/UserDataManager.h"

enum class ROOM_STATE {
	//ROOM_STATE_VOID,	// �Ҵ縸 �Ǿ��ְ�, �� ����..! DEV_55
	ROOM_STATE_SOLO,	// �濡 �Ѹ��� ������� ����
	ROOM_STATE_PLAY		// ������ �÷��� ���� ����
};

struct RoomDynamicData
{
	int hostMissionIndex;
	int guestMissionIndex;
	int subMissionIndex;

	int hostCharacterIndex;
	int guestCharacterIndex;

	// For Packing! --> bool��� ����(������ ����) Data Structure ������ int�� ��ȯ�Ͽ� ����..! 
	// �̿�����, �������� �Ǹ鼭 ũ�� ������� �ѵ�..?
	bool isHostFirst;

	//bool hostUserReady;
	//bool guestUserReady;
	//atomic<int> userReadyCount;

	RoomDynamicData()
		: hostMissionIndex(rand() % 5), guestMissionIndex(rand() % 5), subMissionIndex(rand() % 5)
		, hostCharacterIndex(1), guestCharacterIndex(1)
		, isHostFirst(rand() % 2)
		
		//, userReadyCount(0)
		//hostUserReady(false), guestUserReady(false)
	{}
};

class GameRoom {
public:
	ROOM_STATE roomState;
	GameRoom* left;
	GameRoom* right;

private:
	rbTreeNode<string, UserData>* pUserNode[2]{};

	RoomDynamicData* roomDynamicData;

	int dataProtocol[2];

	// ������ ��Ƶδ� ��.
	char dataBuffer[2][100];

	// �Ⱦ���� ����..
	//BaseStruct* oldhostDataBuffer;
	//BaseStruct* oldguestDataBuffer;
public:
	GameRoom(rbTreeNode<string, UserData>* InHostUserIter, GameRoom* InLeft, GameRoom* InRight);
	//for pDoor
	GameRoom(const int& InBuffer);
	~GameRoom();

	//GameRoom(const GameRoom&) = delete; 
	GameRoom & operator=(const GameRoom&) = delete;

public:

	void JoinRoom(rbTreeNode<string, UserData>* InGuestUserIter);
	
	__inline void DeleteDynamicData() { delete roomDynamicData;  roomDynamicData = nullptr; }

	//new Function
public:
	__inline void GetRoomGameData(const bool& InIsHost, int& retIsHostFirst, int& retPlayerMissionIndex, int& retEnemyMissionIndex, int& retSubMissionIndex)
	{
		if (InIsHost)
		{
			retPlayerMissionIndex = roomDynamicData->hostMissionIndex;
			retEnemyMissionIndex = roomDynamicData->guestMissionIndex;
		}
		else
		{
			retPlayerMissionIndex = roomDynamicData->guestMissionIndex;
			retEnemyMissionIndex = roomDynamicData->hostMissionIndex;
		}

		if (roomDynamicData->isHostFirst)
		{
			retIsHostFirst = 1;
		}
		else
		{
			retIsHostFirst = 0;
		}

		retSubMissionIndex = roomDynamicData->subMissionIndex;

		return;
	}

	__inline void SetCharacterIndex(const bool& InIsHost, const int& InCharacterIndex)
	{
		if (InIsHost)
		{
			roomDynamicData->hostCharacterIndex = InCharacterIndex;
		}
		else
		{
			roomDynamicData->guestCharacterIndex = InCharacterIndex;
		}
	}

	__inline int GetEnemyCharacterIndex(const bool& InIsHost)
	{
		if (InIsHost)
		{
			return roomDynamicData->guestCharacterIndex;
		}
		else
		{
			return roomDynamicData->hostCharacterIndex;
		}
	}

	__inline bool RoomOut(const bool& isHost)
	{
		// DEV_55 ����.

		//if (roomState == ROOM_STATE::ROOM_STATE_WAIT)
		//{
		//	//hostDataProtocol = DISCONNECTED_ENEMY_CLIENT;
		//	//guestDataProtocol = DISCONNECTED_ENEMY_CLIENT;
		//	dataProtocol[0] = DISCONNECTED_ENEMY_CLIENT;
		//	dataProtocol[1] = DISCONNECTED_ENEMY_CLIENT;
		//
		//	return false;
		//}
		//else if (roomState == ROOM_STATE::ROOM_STATE_PLAY)
		//{
		//	//hostDataProtocol = DISCONNECTED_ENEMY_CLIENT;
		//	//guestDataProtocol = DISCONNECTED_ENEMY_CLIENT;
		//
		//	// ���⼭ ���� ��� ó��...!
		//	dataProtocol[0] = DISCONNECTED_ENEMY_CLIENT;
		//	dataProtocol[1] = DISCONNECTED_ENEMY_CLIENT;
		//
		//	if (isHost)
		//	{
		//		(*pUserNode[0]).
		//	}
		//	else
		//	{
		//		RetEnemyIndex = userIndex[0];
		//	}
		//
		//	return true;
		//}
	}

	__inline rbTreeNode<string, UserData>*  RetEnemyUserIter(const bool& InIsHost)
	{
		return pUserNode[InIsHost];
	}

	__inline bool GetGameReady() const
	{
		if (roomState == ROOM_STATE::ROOM_STATE_PLAY)
			return true;

		return false;
	}

public:
	__inline void SetDataBuffer(const bool& InIsHost, const char* InBuffer, const int& InCopySize)
	{
		memcpy(dataBuffer[!InIsHost], InBuffer, InCopySize);
	}

	__inline void GetDataBuffer(const bool& InIsHost, char* RetBuffer, const int& InCopySize)
	{
		memcpy(RetBuffer, dataBuffer[InIsHost], InCopySize);
	}

	__inline void GetDataBuffer(const bool& InIsHost, char* RetBuffer)
	{
		memcpy(RetBuffer, dataBuffer[InIsHost], (int)dataBuffer[InIsHost] + sizeof(int));
	}

	__inline int GetDataProtocol(const bool& InIsHost) const
	{
		return dataProtocol[InIsHost];
	}
	//enum�� �޴������� �ѹ� ���������ϱ�, ����� ���۷������� ���� ������?
	__inline void SetDataProtocol(const bool& InIsHost, const int& InNewDataProtocol)
	{
		dataProtocol[!InIsHost] = InNewDataProtocol;
	}

	__inline void SetFriendUserPtr(rbTreeNode<string, UserData>* InPtr)
	{
		pUserNode[1] = InPtr;
	}

	__inline void SetUserPtr(const bool& InIsGuest, rbTreeNode<string, UserData>* InPtr)
	{
		pUserNode[InIsGuest] = InPtr;
	}
};

