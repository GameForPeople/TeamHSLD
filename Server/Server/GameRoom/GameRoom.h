#pragma once

#include "../stdafx.h"
#include "../Protocol/CommunicationProtocol.h"

#define ROOM_MAX 500

enum class ROOM_STATE {
	ROOM_STATE_VOID		,	// �Ҵ縸 �Ǿ��ְ�, �� ����..!
	ROOM_STATE_SOLO		,	// �濡 �Ѹ��� ������� ����
	ROOM_STATE_WAIT		,	// ���� �� ����, �÷��̸� ��� ���� ����
	ROOM_STATE_PLAY			// ������ �÷��� ���� ����
};

class GameRoom {
	// �ϴ� �� �ۺ�!! --> ��ģ�� // �� �츮 ���̿� �� ���ɤ�����
	GameRoom(const GameRoom&); 
	GameRoom& operator=(const GameRoom&);
public:
	ROOM_STATE roomState;
	
	//std::atomic<bool> isOnRenew[2]{ false };
	//bool isOnExit{ false };	// �� ���� ���� ���! true�� ó������.
	int	userIndex[2]{};
	std::atomic<int> dataProtocol{};

	bool isOnReady[2]{false}; //0�� Host, 1�� Guest

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
		//isOnRenew[0] = true; // ���� ����� ���忡���� �׻� 0�� ����־����!
		//�̰� ����ٰ� ���� ����, �����͸� ������ �Ҷ��� ����.
	}

	__inline void JoinRoom(const int InGuestIndex)
	{
		// Ŭ�󿡼� JoinRoom���ڸ���, Ŭ����ü �ٷ� �ε� ����, ȣ��Ʈ�� �𸣴ϱ� On������
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
			// ���߿� �Ӱ迵�� �ɾ��ֱ� �ؾ���.
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
			return 3; //�� �� �ִ� ���� ����
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
	// ���۷��� �������� �ϴ���. ������ �ܰ迡�� ������ �����䱛 �����ؾ���
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
		// �� �̰� ����ü ���� �����̳�... ��鼭 �̷��� ó������ ��ģ���
		rooms[InRoomIndex].dataBuffer = &InBaseStruct;
	}

	// Caution!! dataBuffer Delete
	void DeleteDataBuffer(const int InRoomIndex)
	{
		delete rooms[InRoomIndex].dataBuffer;
	}

};
