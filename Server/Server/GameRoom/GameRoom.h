#pragma once

#include "../stdafx.h"
#include "../Protocol/CommunicationDataStruct.h"

#define ROOM_MAX 500

enum class ROOM_STATE {
	ROOM_STATE_VOID		,	// 할당만 되어있고, 방 없어..!
	ROOM_STATE_SOLO		,	// 방에 한명이 대기중인 상태
	ROOM_STATE_WAIT		,	// 방이 꽉 차고, 플레이를 대기 중인 상태
	ROOM_STATE_PLAY			// 게임이 플레이 중인 상태
};

class GameRoom {
public:
	ROOM_STATE roomState;
private:
	int	userIndex[2]{};

	int hostMissionIndex;
	int guestMissionIndex;
	int subMissionIndex;

	int hostCharacterIndex;
	int guestCharacterIndex;

	// For Packing! --> bool고냥 쓰고(데이터 절약) Data Structure 에서만 int로 변환하여 전송..! 
	bool isHostFirst;

	int dataProtocol[2];
	
	// 데이터 담아두는 곳.
	char DataBuffer[2][100];

private:

	// error
	//std::atomic<int> dataProtocol;

	// 안쓰기로 결정..
	//BaseStruct* oldhostDataBuffer;
	//BaseStruct* oldguestDataBuffer;

public:
	//GameRoom(const GameRoom&) = delete; 
	GameRoom& operator=(const GameRoom&) = delete;

	__inline GameRoom() : roomState(ROOM_STATE::ROOM_STATE_VOID), //hostDataBuffer(nullptr), guestDataBuffer(nullptr), hostDataProtocol(501), guestDataProtocol(501),
		hostMissionIndex(), guestMissionIndex(), subMissionIndex(), hostCharacterIndex(), guestCharacterIndex(), isHostFirst(),
		dataProtocol() 
	{}

	//__inline ~GameRoom() = default;
	__inline ~GameRoom()
	{
		//if (hostDataBuffer != nullptr)
		//{
		//	delete hostDataBuffer;
		//}
		//
		//if (guestDataBuffer != nullptr)
		//{
		//	delete guestDataBuffer;
		//}
	}

	__inline void CreateRoom(const int& InHostIndex)
	{
		userIndex[0] = InHostIndex;
		roomState = ROOM_STATE::ROOM_STATE_SOLO;
		//isOnRenew[0] = true; // 방을 만드는 입장에서는 항상 0이 비어있어야함!
		//이걸 여기다가 쓰지 말고, 데이터를 보내야 할때만 쓰자.

		hostMissionIndex = rand() % 5;
		guestMissionIndex = rand() % 5;
		subMissionIndex = rand() % 5;

		hostCharacterIndex = 1;
		guestCharacterIndex = 1;

		isHostFirst = rand() % 2;

		//hostDataProtocol = 501;
		//guestDataProtocol = 501;
		//
		//if (hostDataBuffer != nullptr)
		//{
		//	delete hostDataBuffer;
		//}
		//
		//if (guestDataBuffer != nullptr)
		//{
		//	delete guestDataBuffer;
		//}
	}

	__inline void JoinRoom(const int& InGuestIndex)
	{
		// 클라에서 JoinRoom하자마자, 클라자체 바로 로딩 들어가고, 호스트는 모르니까 On시켜줌
		//if (roomState == ROOM_STATE::ROOM_STATE_SOLO) {

		//hostDataProtocol = VOID_GAME_STATE;
		//guestDataProtocol = VOID_GAME_STATE;

		dataProtocol[0] = VOID_GAME_STATE;
		dataProtocol[1] = VOID_GAME_STATE;

		userIndex[1] = InGuestIndex;
		roomState = ROOM_STATE::ROOM_STATE_WAIT;
		//}
	}

	__inline void ExitRoom()
	{
		//? 엥 이거밖에 없네..?
		roomState = ROOM_STATE::ROOM_STATE_VOID;
	}

//new Function
public:
	__inline bool GetGameReady() const
	{
		if (roomState == ROOM_STATE::ROOM_STATE_WAIT)
			return true;

		return false;
	}

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

	__inline bool SignOut(const bool& isHost, int& RetEnemyIndex)
	{
		if (roomState == ROOM_STATE::ROOM_STATE_SOLO)
		{
			roomState = ROOM_STATE::ROOM_STATE_VOID; // 여기서는 이걸 해도 되지만, 아래에서는 이러면 클남! 방나가는거 처리.
			return false;
		}
		else if (roomState == ROOM_STATE::ROOM_STATE_WAIT)
		{
			//hostDataProtocol = DISCONNECTED_ENEMY_CLIENT;
			//guestDataProtocol = DISCONNECTED_ENEMY_CLIENT;
			dataProtocol[0] = DISCONNECTED_ENEMY_CLIENT;
			dataProtocol[1] = DISCONNECTED_ENEMY_CLIENT;

			return false;
		}
		else if (roomState == ROOM_STATE::ROOM_STATE_PLAY)
		{
			//hostDataProtocol = DISCONNECTED_ENEMY_CLIENT;
			//guestDataProtocol = DISCONNECTED_ENEMY_CLIENT;
			// 여기서 게임 결과 처리...!
			dataProtocol[0] = DISCONNECTED_ENEMY_CLIENT;
			dataProtocol[1] = DISCONNECTED_ENEMY_CLIENT;

			if (isHost)
			{
				RetEnemyIndex = userIndex[1];
			}
			else
			{
				RetEnemyIndex = userIndex[0];
			}

			return true;
		}
	}

	__inline int GetEnemyUserIndex(const bool& InIsHost) const
	{
		return userIndex[InIsHost];
	}

public:
	__inline void SetDataBuffer(const bool& InIsHost, const char* InBuffer, const int& InCopySize)
	{
		memcpy(DataBuffer[!InIsHost], InBuffer, InCopySize);
	}

	__inline void GetDataBuffer(const bool& InIsHost, char* RetBuffer, const int& InCopySize)
	{
		memcpy(RetBuffer, DataBuffer[InIsHost], InCopySize);
	}

	__inline void GetDataBuffer(const bool& InIsHost, char* RetBuffer)
	{
		memcpy(RetBuffer, DataBuffer[InIsHost], (int)DataBuffer[InIsHost] + sizeof(int));
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


