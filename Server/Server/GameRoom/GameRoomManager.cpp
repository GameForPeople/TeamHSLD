#include "../GameRoom/GameRoomManager.h"
#include "../IOCPServer/SocketInfo.h"


void GameRoomManager::CreateRoom(user_iter* InUserIter, GameRoom* RetRoomPointer)
{
	RetRoomPointer = new GameRoom(InUserIter, nullptr, nullptr);
	waitRoomCont.Create(RetRoomPointer);
}

void GameRoomManager::JoinRoom(user_iter* InUserIter, GameRoom* RetRoomPointer)
{
	// Push -> Pop을 먼저 하고 (criticalSection을 걸고) 빼고 넣은 방에다가, 그 후 설정까지 끝내고 방 바꿈.
	waitRoomCont.GetOneRoom(RetRoomPointer);
	playRoomCont.Push(RetRoomPointer);

	RetRoomPointer->JoinRoom(InUserIter);
}

void GameRoomManager::DestroyRoom(SocketInfo* ptr)
{
	if (ptr->pRoomIter->GetGameReady())
	{
		playRoomCont.Pop(ptr->pRoomIter);
	}
	else
	{
		waitRoomCont.Pop(ptr->pRoomIter);
	}

	delete (ptr->pRoomIter);

	ptr->pRoomIter = nullptr;
}

bool GameRoomManager::RandomMatchingProcess(user_iter* InUserIter, GameRoom* RetRoomPointer)
{
	if (waitRoomCont.IsEmpty()) // Create!
	{
		CreateRoom(InUserIter, RetRoomPointer);
		return true;
	}
	else // Join
	{
		JoinRoom(InUserIter, RetRoomPointer);
		return false;
	}
}

