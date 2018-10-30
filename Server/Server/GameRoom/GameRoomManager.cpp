#include "GameRoomManager.h"

void GameRoomManager::CreateRoom(user_iter* InUserIter, NewGameRoom* RetRoomPointer)
{
	RetRoomPointer = new NewGameRoom(InUserIter, nullptr, nullptr);
	waitRoomCont.Create(RetRoomPointer);
}

void GameRoomManager::JoinRoom(user_iter* InUserIter, NewGameRoom* RetRoomPointer)
{
	waitRoomCont.GetOneRoom(RetRoomPointer);
	RetRoomPointer->JoinRoom(InUserIter);
	playRoomCont.Push(RetRoomPointer);
}

