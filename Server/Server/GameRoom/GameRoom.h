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


	__inline void CreateRoom(const int& InHostIndex)
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

	__inline void JoinRoom(const int& InGuestIndex)
	{
		// Ŭ�󿡼� JoinRoom���ڸ���, Ŭ����ü �ٷ� �ε� ����, ȣ��Ʈ�� �𸣴ϱ� On������
		//if (roomState == ROOM_STATE::ROOM_STATE_SOLO) {

		userIndex[1] = InGuestIndex;
		roomState = ROOM_STATE::ROOM_STATE_WAIT;
		//}
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

	__inline int& GetEnemyCharacterIndex(const bool& InIsHost)
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
			roomState = ROOM_STATE::ROOM_STATE_VOID; // ���⼭�� �̰� �ص� ������, �Ʒ������� �̷��� Ŭ��.
			return false;
		}
		else if (roomState == ROOM_STATE::ROOM_STATE_WAIT)
		{
			hostDataProtocol = DISCONNECTED_ENEMY_CLIENT;
			guestDataProtocol = DISCONNECTED_ENEMY_CLIENT;

			return false;
		}
		else if (roomState == ROOM_STATE::ROOM_STATE_PLAY)
		{
			hostDataProtocol = DISCONNECTED_ENEMY_CLIENT;
			guestDataProtocol = DISCONNECTED_ENEMY_CLIENT;
			// ���⼭ ���� ��� ó��...!

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
};


