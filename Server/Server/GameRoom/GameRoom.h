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
	// 일단 다 퍼블릭!! --> 미친놈 // 야 우리 사이에 뭘 숨곃ㅎㅎㅎ
	GameRoom(const GameRoom&); 
	GameRoom& operator=(const GameRoom&);
public:
	ROOM_STATE roomState;
	
	//std::atomic<bool> isOnRenew[2]{ false };
	//bool isOnExit{ false };	// 한 명이 나갈 경우! true로 처리해줌.
	int	userIndex[2]{};
	std::atomic<int> dataProtocol{};

	bool isOnReady[2]{false}; //0이 Host, 1이 Guest

	BaseStruct* dataBuffer;

	__inline GameRoom() : roomState(ROOM_STATE::ROOM_STATE_VOID), dataBuffer(nullptr), dataProtocol(0)
	{};

	//__inline ~GameRoom() = default;
	__inline ~GameRoom()
	{
		if (dataBuffer != nullptr)
		{
			delete dataBuffer;
		}
	}

	__inline void CreateRoom(const int InHostIndex)
	{

		userIndex[0] = InHostIndex;
		roomState = ROOM_STATE::ROOM_STATE_SOLO;
		//isOnRenew[0] = true; // 방을 만드는 입장에서는 항상 0이 비어있어야함!
		//이걸 여기다가 쓰지 말고, 데이터를 보내야 할때만 쓰자.
	}

	__inline void JoinRoom(const int InGuestIndex)
	{
		// 클라에서 JoinRoom하자마자, 클라자체 바로 로딩 들어가고, 호스트는 모르니까 On시켜줌
		//if (roomState == ROOM_STATE::ROOM_STATE_SOLO) {

		userIndex[1] = InGuestIndex;
		isOnReady[0] = true;
		roomState = ROOM_STATE::ROOM_STATE_WAIT;
		//}
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

	__inline bool GetAndSetReadyData(const int InRoomIndex, const bool InIsHost)
	{
		if (InIsHost) 
		{
			if (rooms[InRoomIndex].isOnReady[0]) 
			{
				rooms[InRoomIndex].isOnReady[0] = false;
				return true;
			}
			return false;
		}
		else
		{
			if (rooms[InRoomIndex].isOnReady[1])
			{
				rooms[InRoomIndex].isOnReady[1] = false;
				return true;
			}
			return false;
		}
	}

//for InGame
public:
	// 레퍼런스 쓰지말자 일단은. 마지막 단계에서 데이터 프로토콣 변경해야지
	// atomic
	int GetDataProtocol(const int InRoomIndex) const
	{
		return rooms[InRoomIndex].dataProtocol;
	}

	// atomic 
	void SetDataProtocol(const int InRoomIndex, const int InNewDataProtocol = 0)
	{
		rooms[InRoomIndex].dataProtocol = InNewDataProtocol;
	}


	BaseStruct* GetDataBuffer(const int InRoomIndex) // const
	{
		return rooms[InRoomIndex].dataBuffer;
	}

	void SetDataBuffer(const int InRoomIndex, BaseStruct InBaseStruct)
	{
		// 야 이게 도대체 무슨 문법이냐... 살면서 이런거 처음본다 미친놈아
		rooms[InRoomIndex].dataBuffer = &InBaseStruct;
	}

	// Caution!! dataBuffer Delete
	void DeleteDataBuffer(const int InRoomIndex)
	{
		delete rooms[InRoomIndex].dataBuffer;
	}

};
