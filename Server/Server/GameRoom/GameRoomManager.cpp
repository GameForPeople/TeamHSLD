#include "GameRoomManager.h"

GameRoomManager::GameRoomManager()
{
	rooms.reserve(ROOM_MAX);

	for (int i = 0; i < ROOM_MAX; ++i)
	{
		rooms.emplace_back();
	}
}

int GameRoomManager::CreateRoom(const int& InHostIndex)
{
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

int GameRoomManager::JoinRoom(const int& InGuestIndex, int& RetRoomIndex)
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

