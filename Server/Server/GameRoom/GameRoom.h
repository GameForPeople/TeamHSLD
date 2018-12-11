#pragma once

#include "../PCH/stdafx.h"
#include "../Protocol/CommunicationProtocol.h"
#include "../UserData/UserDataManager.h"

enum class ROOM_STATE {
	//ROOM_STATE_VOID,	// �Ҵ縸 �Ǿ��ְ�, �� ����..! DEV_55
	ROOM_STATE_SOLO,	// �濡 �Ѹ��� ������� ����
	ROOM_STATE_PLAY		// ������ �÷��� ���� ����
};

struct RoomDynamicData
{
	Type_Nickname hostNickname;
	Type_Nickname guestNickname;

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

	// DEV_66 �۾� �� �߰�, ģ�� �ʴ�� ���� ������ ��츦 ���� ����ϸ�, �ش��� False�� Guest�� �ٲ� ���, �ʴ븦 ������ ��Ȳ��.
	bool friendInviteBuffer;	// 1����Ʈ�̸� �������� ����ϱ� ������, ģ���� �ƴ� ��쿡�� �� ū ������� ����� �� ���� ��.

	RoomDynamicData(const Type_Nickname& InHostNickName)
		: hostMissionIndex(rand() % 5), guestMissionIndex(rand() % 5), subMissionIndex(rand() % 5)
		, hostCharacterIndex(1), guestCharacterIndex(1)
		, isHostFirst(rand() % 2)
		, friendInviteBuffer(true)
		//, userReadyCount(0)
		//, hostUserReady(false), guestUserReady(false)
	{}
};

class GameRoom {
public:
	ROOM_STATE roomState;
	
	/*
	GameRoom* left;
	GameRoom* right;
	*/

private:
	shared_ptr<RoomDynamicData> roomDynamicData;

	int dataProtocol[2];

	// ������ ��Ƶδ� ��.
	char dataBuffer[2][100];

	bool isFriendMode;

	// �Ⱦ���� ����..
	//BaseStruct* oldhostDataBuffer;
	//BaseStruct* oldguestDataBuffer;

public:
	GameRoom(const shared_ptr<UserData>& InHostUserIter, /*GameRoom* InLeft, GameRoom* InRight,*/ bool InIsFriendMode = false );
	
	GameRoom() = delete;
	~GameRoom();

	//GameRoom(const GameRoom&) = delete; 
	//GameRoom & operator=(const GameRoom&) = delete;

public:
	void JoinRoom(const shared_ptr<UserData>& InGuestUserIter);
	
	__inline void DeleteDynamicData() noexcept { roomDynamicData.reset(); }

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

	//new Function
public:
	__inline void GetRoomGameData(const bool& InIsHost, int& retIsHostFirst, int& retPlayerMissionIndex, 
		int& retEnemyMissionIndex, int& retSubMissionIndex)
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

	//DEV_66 �г�����, ���̳��� �����Ϳ� �߰��Ǹ鼭, Iter���, �г��Ӹ� ��û�ϴ� ������ ����.
	__inline void GetRoomGameDataWithNickname(const bool& InIsHost, int& retIsHostFirst, int& retPlayerMissionIndex,
		int& retEnemyMissionIndex, int& retSubMissionIndex, Type_Nickname& retEnemyNickname)
	{
		if (InIsHost)
		{
			retPlayerMissionIndex = roomDynamicData->hostMissionIndex;
			retEnemyMissionIndex = roomDynamicData->guestMissionIndex;
			retEnemyNickname = roomDynamicData->guestNickname;
		}
		else
		{
			retPlayerMissionIndex = roomDynamicData->guestMissionIndex;
			retEnemyMissionIndex = roomDynamicData->hostMissionIndex;
			retEnemyNickname = roomDynamicData->hostNickname;
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

	//DEV_66
	_NODISCARD __inline Type_Nickname& GetEnemyNickname(const bool& InIsHost) const noexcept
	{
		if (InIsHost)
		{
			return roomDynamicData->guestNickname;
		}
		else
		{
			return roomDynamicData->hostNickname;
		}
	}

	//DEV_66 For Friend
	_NODISCARD __inline bool GetDynamicFriendInviteBuffer() const { return roomDynamicData->friendInviteBuffer; }
	
	_NODISCARD __inline bool GetIsFriendMode() const noexcept { return isFriendMode; }

	__inline void SetDynamicFriendInviteBuffer(/*only false*/) { roomDynamicData->friendInviteBuffer = false; }

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

	 _NODISCARD __inline int GetEnemyCharacterIndex(const bool& InIsHost)
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

	__inline bool GetGamePlay() const
	{
		if (roomState == ROOM_STATE::ROOM_STATE_PLAY)
			return true;

		return false;
	}

public:
	__inline void SetDataBuffer(const bool& InIsHost, const char* InBuffer, const int& InCopySize) noexcept
	{
		memcpy(dataBuffer[!InIsHost], InBuffer, InCopySize);
	}

	__inline void GetDataBuffer(const bool& InIsHost, char* RetBuffer, const int& InCopySize) noexcept
	{
		memcpy(RetBuffer, dataBuffer[InIsHost], InCopySize);
	}
	__inline void GetDataBuffer(const bool& InIsHost, char* RetBuffer) noexcept
	{
		memcpy(RetBuffer, dataBuffer[InIsHost], (int)dataBuffer[InIsHost] + sizeof(int));
	}

	_NODISCARD __inline int GetDataProtocol(const bool& InIsHost) const noexcept
	{
		return dataProtocol[InIsHost];
	}

	//enum�� �޴������� �ѹ� ���������ϱ�, ����� ���۷������� ���� ������?
	__inline void SetDataProtocol(const bool& InIsHost, const int& InNewDataProtocol) noexcept
	{
		dataProtocol[!InIsHost] = InNewDataProtocol;
	}

#pragma region [Old Function]

	//__inline rbTreeNode<string, UserData>*  RetEnemyUserIter(const bool& InIsHost)
	//{
	//	return pUserNode[InIsHost];
	//}
	//__inline void SetFriendUserPtr(rbTreeNode<string, UserData>* InPtr)
	//{
	//	pUserNode[1] = InPtr;
	//}
	//
	//__inline void SetUserPtr(const bool& InIsGuest, rbTreeNode<string, UserData>* InPtr)
	//{
	//	pUserNode[InIsGuest] = InPtr;
	//}
#pragma endregion
};

