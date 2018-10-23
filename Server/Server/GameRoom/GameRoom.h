#pragma once

#include "../stdafx.h"
#include "../Protocol/CommunicationDataStruct.h"

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
private:
	int	userIndex[2]{};

	int hostMissionIndex;
	int guestMissionIndex;
	int subMissionIndex;

	int hostCharacterIndex;
	int guestCharacterIndex;

	// For Packing! --> bool��� ����(������ ����) Data Structure ������ int�� ��ȯ�Ͽ� ����..! 
	bool isHostFirst;

	int dataProtocol[2];
	
	// ������ ��Ƶδ� ��.
	char DataBuffer[2][100];

private:

	// error
	//std::atomic<int> dataProtocol;

	// �Ⱦ���� ����..
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
		//isOnRenew[0] = true; // ���� ����� ���忡���� �׻� 0�� ����־����!
		//�̰� ����ٰ� ���� ����, �����͸� ������ �Ҷ��� ����.

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
		// Ŭ�󿡼� JoinRoom���ڸ���, Ŭ����ü �ٷ� �ε� ����, ȣ��Ʈ�� �𸣴ϱ� On������
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
		//? �� �̰Źۿ� ����..?
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
			roomState = ROOM_STATE::ROOM_STATE_VOID; // ���⼭�� �̰� �ص� ������, �Ʒ������� �̷��� Ŭ��! �泪���°� ó��.
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
			// ���⼭ ���� ��� ó��...!
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
														//enum�� �޴������� �ѹ� ���������ϱ�, ����� ���۷������� ���� ������?
	__inline void SetDataProtocol(const bool& InIsHost, const int& InNewDataProtocol)
	{
		dataProtocol[!InIsHost] = InNewDataProtocol;
	}
};


