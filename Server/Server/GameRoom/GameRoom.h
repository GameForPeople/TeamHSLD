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
public:
	ROOM_STATE roomState;
	
	int	userIndex[2]{};

	int hostMissionIndex;
	int guestMissionIndex;
	int subMissionIndex;

	int hostCharacterIndex;
	int guestCharacterIndex;

	// For Packing! --> bool��� ����(������ ����) Data Structure ������ int�� ��ȯ�Ͽ� ����..! 
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
		//isOnRenew[0] = true; // ���� ����� ���忡���� �׻� 0�� ����־����!
		//�̰� ����ٰ� ���� ����, �����͸� ������ �Ҷ��� ����.

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
		// Ŭ�󿡼� JoinRoom���ڸ���, Ŭ����ü �ٷ� �ε� ����, ȣ��Ʈ�� �𸣴ϱ� On������
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

	__inline bool GetGameReady(const int& InRoomIndex)
	{
		return (rooms[InRoomIndex].GetGameReady());
	}

//for InGame
public:
	// ���۷��� �������� �ϴ���. ������ �ܰ迡�� ������ �����䱛 �����ؾ���
	// atomic
	int GetDataProtocol(const bool& InIsHost, const int& InRoomIndex) const
	{
		if(InIsHost)
			return rooms[InRoomIndex].guestDataProtocol;
		else 
			return rooms[InRoomIndex].hostDataProtocol;
	}
	
	// atomic                                                             // ���۷��� ���� enum ���޴´�.
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
		// �� �̰� ����ü ���� �����̳�... ��鼭 �̷��� ó������ ��ģ���
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
