#pragma once

#include "../stdafx.h"
#include "../Protocol/CommunicationProtocol.h"

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
	
	int	userIndex[2]{};

	int hostMissionIndex;
	int guestMissionIndex;
	int subMissionIndex;

	int hostCharacterIndex;
	int guestCharacterIndex;

	// For Packing! --> bool고냥 쓰고(데이터 절약) Data Structure 에서만 int로 변환하여 전송..! 
	bool isHostFirst;
	//int isHostFirst;

	// error
	//std::atomic<int> dataProtocol;

	int hostDataProtocol;
	int guestDataProtocol;
	
	BaseStruct* hostDataBuffer;
	BaseStruct* guestDataBuffer;

public:
	//GameRoom(const GameRoom&) = delete; 
	GameRoom& operator=(const GameRoom&) = delete;

	__inline GameRoom() : roomState(ROOM_STATE::ROOM_STATE_VOID), hostDataBuffer(nullptr), guestDataBuffer(nullptr), hostDataProtocol(0), guestDataProtocol(0)
		, hostMissionIndex(), guestMissionIndex(), subMissionIndex(), hostCharacterIndex(), guestCharacterIndex(), isHostFirst()
	{}

	//__inline ~GameRoom() = default;
	__inline ~GameRoom()
	{
		if (hostDataBuffer != nullptr)
		{
			delete hostDataBuffer;
		}

		if (guestDataBuffer != nullptr)
		{
			delete guestDataBuffer;
		}
	}

	__inline void CreateRoom(const int InHostIndex)
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

		hostDataProtocol = 0;
		guestDataProtocol = 0;

		if (hostDataBuffer != nullptr)
		{
			delete hostDataBuffer;
		}

		if (guestDataBuffer != nullptr)
		{
			delete guestDataBuffer;
		}
	}

	__inline void JoinRoom(const int InGuestIndex)
	{
		// 클라에서 JoinRoom하자마자, 클라자체 바로 로딩 들어가고, 호스트는 모르니까 On시켜줌
		//if (roomState == ROOM_STATE::ROOM_STATE_SOLO) {

		userIndex[1] = InGuestIndex;
		roomState = ROOM_STATE::ROOM_STATE_WAIT;
		//}
	}


//new Function
public:
	__inline bool GetGameReady()
	{
		if (roomState == ROOM_STATE::ROOM_STATE_WAIT)
			return true;

		return false;
	}

	void GetRoomGameData(const bool& InIsHost, int& retIsHostFirst, int& retPlayerMissionIndex, int& retEnemyMissionIndex, int& retSubMissionIndex)
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

	void SetCharacterIndex(const bool& InIsHost, const int& InCharacterIndex)
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

	int& GetEnemyCharacterIndex(const bool& InIsHost)
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
};

class CGameRoom {

	std::vector<GameRoom> rooms;

public:

	__inline CGameRoom()
	{
		rooms.reserve(ROOM_MAX);

		for (int i = 0; i < ROOM_MAX; ++i)
		{
			rooms.emplace_back();
		}
	}

	__inline ~CGameRoom() = default;

	int CreateRoom(const int InHostIndex) {
		for (int i = 0; i < ROOM_MAX; ++i) 
		{
			// 나중에 임계영역 걸어주긴 해야함.
			if (rooms[i].roomState == ROOM_STATE::ROOM_STATE_VOID)
			{
				rooms[i].CreateRoom(InHostIndex);

				return i; // roomIndex;
			}
		}
	}

	int JoinRoom(const int InGuestIndex, int& RetRoomIndex)
	{
		if (RetRoomIndex == -1) 
		{
			for (RetRoomIndex = 0; RetRoomIndex < ROOM_MAX; ++RetRoomIndex)
			{
				if (rooms[RetRoomIndex].roomState == ROOM_STATE::ROOM_STATE_SOLO)
				{
					rooms[RetRoomIndex].JoinRoom(InGuestIndex);

					return 0;
				}
			}
			return 3; //들어갈 수 있는 방이 없음
		}
		else 
		{
			if (rooms[RetRoomIndex].roomState == ROOM_STATE::ROOM_STATE_SOLO)
			{
				rooms[RetRoomIndex].JoinRoom(InGuestIndex);

				return 0;
			}
			else if (rooms[RetRoomIndex].roomState == ROOM_STATE::ROOM_STATE_VOID)
			{
				return 1;
			}
			else if (rooms[RetRoomIndex].roomState == ROOM_STATE::ROOM_STATE_PLAY 
			|| rooms[RetRoomIndex].roomState == ROOM_STATE::ROOM_STATE_WAIT)
			{
				return 2;
			}
		}
	}

	int GetEnemyIndex(const int InRoomIndex, const bool InIsHost) 
	{
		if (InIsHost)
			return rooms[InRoomIndex].userIndex[1];
		else
			return rooms[InRoomIndex].userIndex[0];
	}

	__inline bool GetGameReady(const int& InRoomIndex)
	{
		return (rooms[InRoomIndex].GetGameReady());
	}

//for InGame
public:
	// 레퍼런스 쓰지말자 일단은. 마지막 단계에서 데이터 프로토콣 변경해야지
	// atomic
	int GetDataProtocol(const bool& InIsHost, const int& InRoomIndex) const
	{
		if(InIsHost)
			return rooms[InRoomIndex].guestDataProtocol;
		else 
			return rooms[InRoomIndex].hostDataProtocol;
	}
	
	// atomic                                                             // 레퍼런스 쓰면 enum 못받는다.
	void SetDataProtocol(const int& InRoomIndex, const bool& InIsHost, const int InNewDataProtocol ) //= 0)
	{
		if (InIsHost)
			rooms[InRoomIndex].hostDataProtocol = InNewDataProtocol;
		else
			rooms[InRoomIndex].guestDataProtocol = InNewDataProtocol;
	}

	BaseStruct* GetDataBuffer(const int& InRoomIndex, const bool& InIsHost) // const
	{
		if (InIsHost)
			return rooms[InRoomIndex].guestDataBuffer;
		else
			return rooms[InRoomIndex].hostDataBuffer;
	}

	void SetDataBuffer(const int& InRoomIndex, const bool& InIsHost, BaseStruct InBaseStruct)
	{
		// 야 이게 도대체 무슨 문법이냐... 살면서 이런거 처음본다 미친놈아
		if (InIsHost)
			rooms[InRoomIndex].hostDataBuffer = &InBaseStruct;
		else
			rooms[InRoomIndex].guestDataBuffer = &InBaseStruct;
	}

	// Caution!! Delete dataBuffer!
	void DeleteDataBuffer(const int& InRoomIndex, const bool& InIsHost)
	{
		if (InIsHost)
			delete rooms[InRoomIndex].guestDataBuffer;
		else
			delete rooms[InRoomIndex].hostDataBuffer;
	}


public:
	// new Function

	// True = Create , False = Join
	bool RandomMatchingProcess(const int InUserIndex, int& RetRoomIndex)
	{
		for (RetRoomIndex = 0; RetRoomIndex < ROOM_MAX; ++RetRoomIndex)
		{
			if (rooms[RetRoomIndex].roomState == ROOM_STATE::ROOM_STATE_SOLO)
			{
				rooms[RetRoomIndex].JoinRoom(InUserIndex);

				return false;
			}
		}

		for (RetRoomIndex = 0; RetRoomIndex < ROOM_MAX; ++RetRoomIndex)
		{
			if (rooms[RetRoomIndex].roomState == ROOM_STATE::ROOM_STATE_VOID)
			{
				rooms[RetRoomIndex].CreateRoom(InUserIndex);

				return true;
			}
		}
	}

	void GetRoomGameData(const int& InRoomIndex, const bool& InIsHost, int& retIsHostFirst , int& retPlayerMissionIndex, int& retEnemyMissionIndex, int& retSubMissionIndex)
	{
		rooms[InRoomIndex].GetRoomGameData(InIsHost, retIsHostFirst, retPlayerMissionIndex, retEnemyMissionIndex, retSubMissionIndex);
	}

	void SetCharacterIndex(const int& InRoomIndex, const bool& InIsHost, const int& InCharacterIndex)
	{
		rooms[InRoomIndex].SetCharacterIndex(InIsHost, InCharacterIndex);
	}

	int& GetEnemyCharacterIndex(const int& InRoomIndex, const bool& InIsHost)
	{
		return rooms[InRoomIndex].GetEnemyCharacterIndex(InIsHost);
	}

private:

};
