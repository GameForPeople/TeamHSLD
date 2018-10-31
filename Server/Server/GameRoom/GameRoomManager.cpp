#include "../GameRoom/GameRoomManager.h"
#include "../IOCPServer/SocketInfo.h"


void GameRoomManager::CreateRoom(user_iter* InUserIter, GameRoom* RetRoomPointer)
{
	RetRoomPointer = new GameRoom(InUserIter, nullptr, nullptr);
	waitRoomCont.Create(RetRoomPointer);
}

void GameRoomManager::JoinRoom(user_iter* InUserIter, GameRoom* RetRoomPointer)
{
	waitRoomCont.GetOneRoom(RetRoomPointer);
	RetRoomPointer->JoinRoom(InUserIter);
	playRoomCont.Push(RetRoomPointer);
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

