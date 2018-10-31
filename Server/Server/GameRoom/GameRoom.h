#pragma once

#include "../PCH/stdafx.h"
#include "../Protocol/CommunicationDataStruct.h"
#include "../UserData/UserDataManager.h"

enum class ROOM_STATE {
	//ROOM_STATE_VOID,	// 할당만 되어있고, 방 없어..! DEV_55
	ROOM_STATE_SOLO,	// 방에 한명이 대기중인 상태
	ROOM_STATE_PLAY		// 게임이 플레이 중인 상태
};

class GameRoom {
public:
	ROOM_STATE roomState;
	GameRoom* left;
	GameRoom* right;

private:
	user_iter* pUserIter[2]{};

	int hostMissionIndex;
	int guestMissionIndex;
	int subMissionIndex;

	int hostCharacterIndex;
	int guestCharacterIndex;

	// For Packing! --> bool고냥 쓰고(데이터 절약) Data Structure 에서만 int로 변환하여 전송..! 
	bool isHostFirst;

	int dataProtocol[2];

	// 데이터 담아두는 곳.
	char dataBuffer[2][100];

	// 안쓰기로 결정..
	//BaseStruct* oldhostDataBuffer;
	//BaseStruct* oldguestDataBuffer;

public:
	GameRoom(user_iter* InHostUserIter, GameRoom* InLeft, GameRoom* InRight);
	//for pDoor
	GameRoom(const int& InBuffer);
	~GameRoom();

	//GameRoom(const GameRoom&) = delete; 
	GameRoom & operator=(const GameRoom&) = delete;

public:

	void JoinRoom(user_iter* InGuestUserIter);
	
	//new Function
public:
	__inline void GetRoomGameData(const bool& InIsHost, int& retIsHostFirst, int& retPlayerMissionIndex, int& retEnemyMissionIndex, int& retSubMissionIndex)
	{
		if (InIsHost)
		{
			retPlayerMissionIndex = hostMissionIndex;
			retEnemyMissionIndex = guestMissionIndex;
		}
		else
		{
			retPlayerMissionIndex = guestMissionIndex;
			retEnemyMissionIndex = hostMissionIndex;
		}

		if (isHostFirst)
		{
			retIsHostFirst = 1;
		}
		else
		{
			retIsHostFirst = 0;
		}

		retSubMissionIndex = subMissionIndex;

		return;
	}

	__inline void SetCharacterIndex(const bool& InIsHost, const int& InCharacterIndex)
	{
		if (InIsHost)
		{
			hostCharacterIndex = InCharacterIndex;
		}
		else
		{
			guestCharacterIndex = InCharacterIndex;
		}
	}

	__inline int GetEnemyCharacterIndex(const bool& InIsHost)
	{
		if (InIsHost)
		{
			return guestCharacterIndex;
		}
		else
		{
			return hostCharacterIndex;
		}
	}

	__inline bool RoomOut(const bool& isHost)
	{
		// DEV_55 제외.

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
		//	// 여기서 게임 결과 처리...!
		//	dataProtocol[0] = DISCONNECTED_ENEMY_CLIENT;
		//	dataProtocol[1] = DISCONNECTED_ENEMY_CLIENT;
		//
		//	if (isHost)
		//	{
		//		(*pUserIter[0]).
		//	}
		//	else
		//	{
		//		RetEnemyIndex = userIndex[0];
		//	}
		//
		//	return true;
		//}
	}

	__inline void RetEnemyUserIter(const bool& InIsHost, user_iter* RetUserIter)
	{
		RetUserIter = pUserIter[InIsHost];
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
	//enum을 메니저에서 한번 복사했으니까, 여기는 레퍼런스여도 되지 않을까?
	__inline void SetDataProtocol(const bool& InIsHost, const int& InNewDataProtocol)
	{
		dataProtocol[!InIsHost] = InNewDataProtocol;
	}
};

