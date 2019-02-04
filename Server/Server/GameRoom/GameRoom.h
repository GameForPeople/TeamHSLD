#pragma once

#include "../PCH/stdafx.h"
#include "../Protocol/CommunicationProtocol.h"
#include "../UserData/UserDataManager.h"

enum class ROOM_STATE {
	//ROOM_STATE_VOID,	// 할당만 되어있고, 방 없어..! DEV_55
	ROOM_STATE_SOLO,	// 방에 한명이 대기중인 상태
	ROOM_STATE_PLAY		// 게임이 플레이 중인 상태
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

	// For Packing! --> bool고냥 쓰고(데이터 절약) Data Structure 에서만 int로 변환하여 전송..! 
	// 이엿지만, 동적으로 되면서 크게 상관없긴 한데..?
	bool isHostFirst;

	//bool hostUserReady;
	//bool guestUserReady;
	//atomic<int> userReadyCount;

	// DEV_66 작업 중 추가, 친구 초대로 인한 게임일 경우를 위해 사용하며, 해당을 False로 Guest가 바꿀 경우, 초대를 거절한 상황임.
	bool friendInviteBuffer;	// 1바이트이며 동적으로 사용하기 때문에, 친구방 아닐 경우에도 뭐 큰 상관없이 사용할 수 있을 듯.

	RoomDynamicData(const Type_Nickname& InHostNickName, const int InHostCharacterIndex) noexcept
		: hostMissionIndex(rand() % 5), guestMissionIndex(rand() % 5), subMissionIndex(rand() % 5)
		, hostCharacterIndex(InHostCharacterIndex), guestCharacterIndex(0)
		, isHostFirst(rand() % 2)
		, friendInviteBuffer(true)
		, hostNickname(InHostNickName)
		, guestNickname()
		//, userReadyCount(0)
		//, hostUserReady(false), guestUserReady(false)
	{}

	~RoomDynamicData() = default;
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

	// 데이터 담아두는 곳.
	char dataBuffer[2][100];

	bool isFriendMode;
	
	BYTE emojiBuffer[2];
	// 안쓰기로 결정..
	//BaseStruct* oldhostDataBuffer;
	//BaseStruct* oldguestDataBuffer;

public:
	GameRoom(const shared_ptr<UserData> InHostUserIter, /*GameRoom* InLeft, GameRoom* InRight,*/ bool InIsFriendMode = false );
	
	GameRoom() = delete;
	~GameRoom();

	//GameRoom(const GameRoom&) = delete; 
	//GameRoom & operator=(const GameRoom&) = delete;

public:
	void JoinRoom(const shared_ptr<UserData> InGuestUserIter);
	
	__inline void DeleteDynamicData() noexcept { roomDynamicData.reset(); }

	//new Function
public:
	__inline void GetRoomGameData(const bool InIsHost, int& retIsHostFirst, int& retPlayerMissionIndex, 
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

	//DEV_66 닉네임이, 다이나믹 데이터에 추가되면서, Iter대신, 닉네임만 요청하는 내용이 적용
	//DEV_80
	__inline void GetRoomGameDataWithNickname(const bool InIsHost, int& retIsHostFirst, int& retPlayerMissionIndex,
		int& retEnemyMissionIndex, int& retSubMissionIndex, int& retEnemyCharacterIndex, Type_Nickname& retEnemyNickname)
	{
		GetRoomGameData(InIsHost, retIsHostFirst, retPlayerMissionIndex, retEnemyMissionIndex, retSubMissionIndex);

		if (InIsHost)
		{
			retEnemyNickname = roomDynamicData->guestNickname;
			retEnemyCharacterIndex = roomDynamicData->guestCharacterIndex;
		}
		else
		{
			retEnemyNickname = roomDynamicData->hostNickname;
			retEnemyCharacterIndex = roomDynamicData->hostCharacterIndex;
		}
		return;
	}

	//DEV_66
	_NODISCARD	__inline Type_Nickname&		GetEnemyNickname(const bool& InIsHost) const noexcept
	{
		if (InIsHost)
			return roomDynamicData->guestNickname;
		else
			return roomDynamicData->hostNickname;
	}
	_NODISCARD	__inline bool				GetDynamicFriendInviteBuffer() const { return roomDynamicData->friendInviteBuffer; }
	_NODISCARD	__inline bool				GetIsFriendMode() const noexcept { return isFriendMode; }
	_NODISCARD	__inline int				GetEnemyCharacterIndex(const bool& InIsHost)
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
	_NODISCARD	__inline bool				GetGamePlay() const noexcept
	{
		if (roomState == ROOM_STATE::ROOM_STATE_PLAY)
			return true;
		return false;
	}

	__inline void							SetDynamicFriendInviteBuffer(/*only false*/) { roomDynamicData->friendInviteBuffer = false; }
	__inline void							SetCharacterIndex(const bool& InIsHost, const int& InCharacterIndex)
	{
		if (InIsHost)
			roomDynamicData->hostCharacterIndex = InCharacterIndex;
		else
			roomDynamicData->guestCharacterIndex = InCharacterIndex;
	}

public:
	__inline void SetDataBuffer(const bool InIsHost, const char* InBuffer, const int InCopySize) noexcept
	{
		memcpy(dataBuffer[!InIsHost], InBuffer, InCopySize);
	}

	__inline void GetDataBuffer(const bool InIsHost, char* RetBuffer, const int InCopySize)  noexcept
	{
		memcpy(RetBuffer, dataBuffer[InIsHost], InCopySize);
	}
	__inline void GetDataBuffer(const bool InIsHost, char* RetBuffer)  noexcept
	{
		memcpy(RetBuffer, dataBuffer[InIsHost], (int)dataBuffer[InIsHost] + sizeof(int));
	}

	_NODISCARD __inline int GetDataProtocol(const bool InIsHost) const noexcept
	{
		return dataProtocol[InIsHost];
	}

	__inline void SetDataProtocol(const bool InIsHost, const int InNewDataProtocol) noexcept
	{
		dataProtocol[!InIsHost] = InNewDataProtocol;
	}

public:
	__inline pair<bool,BYTE> GetEmoji(const bool InIsHost)
	{
		pair<bool, BYTE> retBuffer(false,0);

		if (emojiBuffer[InIsHost] != 0)
		{
			retBuffer.first = true;
			retBuffer.second = emojiBuffer[InIsHost];
			emojiBuffer[InIsHost] = 0;
		}

		return retBuffer;
	}

	__inline void SetEmoji(const bool InIsHost, const BYTE InImoji) noexcept
	{
		emojiBuffer[!InIsHost] = InImoji;
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

