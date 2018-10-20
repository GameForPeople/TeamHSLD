#pragma once

#include "../GameRoom/GameRoom.h"

class GameRoomManager {
	std::vector<GameRoom> rooms;

public:
	GameRoomManager();

	__inline ~GameRoomManager() = default;

public:
	int CreateRoom(const int& InHostIndex);

	int JoinRoom(const int& InGuestIndex, int& RetRoomIndex);

	__inline void ExitRoom(const int& InRoomIndex)
	{
		rooms[InRoomIndex].ExitRoom();
	}

	__inline int GetEnemyIndex(const int& InRoomIndex, const bool& InIsHost) const
	{
		return rooms[InRoomIndex].GetEnemyUserIndex(InIsHost);
		//if (InIsHost)
		//	return rooms[InRoomIndex].userIndex[1];
		//else
		//	return rooms[InRoomIndex].userIndex[0];
	}

	__inline bool GetGameReady(const int& InRoomIndex) const
	{
		return (rooms[InRoomIndex].GetGameReady());
	}

public: //for InGameScene

	// ���۷��� �������� �ϴ���. ������ �ܰ迡�� ������ �����䱛 �����ؾ��� --> �̰� ���������� �޾Ƴ��� ����????
	// atomic --> �̿��ٰ� �ƴ�!
	__inline int GetDataProtocol(const int& InRoomIndex, const bool& InIsHost) const
	{
		return rooms[InRoomIndex].GetDataProtocol(InIsHost);
	}

	// atomic  -> �̿��ٰ� �ƴ�                                                  // ���۷��� ���� enum ���޴´�.
	__inline void SetDataProtocol(const int& InRoomIndex, const bool& InIsHost, const int InNewDataProtocol) //= 0)
	{
		rooms[InRoomIndex].SetDataProtocol(InIsHost, InNewDataProtocol);
	}

	//BaseStruct* GetDataBuffer(const int& InRoomIndex, const bool& InIsHost) // const
	//{
	//	if (InIsHost)
	//		return rooms[InRoomIndex].guestDataBuffer;
	//	else
	//		return rooms[InRoomIndex].hostDataBuffer;
	//}

	//void SetDataBuffer(const int& InRoomIndex, const bool& InIsHost, BaseStruct* InBaseStruct)
	//{
	//	// �� �̰� ����ü ���� �����̳�... ��鼭 �̷��� ó������ ��ģ���
	//	if (InIsHost)
	//		rooms[InRoomIndex].hostDataBuffer = InBaseStruct;
	//	else
	//		rooms[InRoomIndex].guestDataBuffer = InBaseStruct;
	//}



	// Caution!! Delete dataBuffer!
	//void DeleteDataBuffer(const int& InRoomIndex, const bool& InIsHost)
	//{
	//	if (InIsHost)
	//		delete rooms[InRoomIndex].guestDataBuffer;
	//	else
	//		delete rooms[InRoomIndex].hostDataBuffer;
	//}


public:// new Function
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

	void GetRoomGameData(const int& InRoomIndex, const bool& InIsHost, int& retIsHostFirst, int& retPlayerMissionIndex, int& retEnemyMissionIndex, int& retSubMissionIndex)
	{
		rooms[InRoomIndex].GetRoomGameData(InIsHost, retIsHostFirst, retPlayerMissionIndex, retEnemyMissionIndex, retSubMissionIndex);
	}

	void SetCharacterIndex(const int& InRoomIndex, const bool& InIsHost, const int& InCharacterIndex)
	{
		rooms[InRoomIndex].SetCharacterIndex(InIsHost, InCharacterIndex);
	}

	int GetEnemyCharacterIndex(const int& InRoomIndex, const bool& InIsHost)
	{
		// �̰� ���۷����� ��ȯ�ص� �ǳ�...?
		return rooms[InRoomIndex].GetEnemyCharacterIndex(InIsHost);
	}

	bool SignOut(const int& InRoomIndex, const bool& InIsHost, int& RetEnemyIndex)
	{
		return rooms[InRoomIndex].SignOut(InIsHost, RetEnemyIndex);
	}

	//newFunction
	void SetDataBuffer(const int& InRoomIndex, const bool& InIsHost, const char* InBuffer, const int& InCopySize)
	{
		rooms[InRoomIndex].SetDataBuffer(InIsHost, InBuffer, InCopySize);
	}

	void GetDataBuffer(const int& InRoomIndex, const bool& InIsHost, char* RetBuffer, const int& InCopySize)
	{
		rooms[InRoomIndex].GetDataBuffer(InIsHost, RetBuffer, InCopySize);
	}

	void GetDataBuffer(const int& InRoomIndex, const bool& InIsHost, char* RetBuffer)
	{
		rooms[InRoomIndex].GetDataBuffer(InIsHost, RetBuffer);
	}
};