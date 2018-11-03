#include "../GameRoom/GameRoomManager.h"
#include "../IOCPServer/SocketInfo.h"


void GameRoomManager::CreateRoom(rbTreeNode<string, UserData>* InUserPtr, GameRoom* RetRoomPtr)
{
	RetRoomPtr = new GameRoom(InUserPtr, nullptr, nullptr);
	waitRoomCont.Create(RetRoomPtr);
}

void GameRoomManager::JoinRoom(rbTreeNode<string, UserData>* InUserPtr, GameRoom* RetRoomPtr)
{
	// Push -> Pop�� ���� �ϰ� (criticalSection�� �ɰ�) ���� ���� �濡�ٰ�, �� �� �������� ������ �� �ٲ�.
	waitRoomCont.GetOneRoom(RetRoomPtr);
	playRoomCont.Push(RetRoomPtr);

	RetRoomPtr->JoinRoom(InUserPtr);
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

bool GameRoomManager::RandomMatchingProcess(rbTreeNode<string, UserData>* InUserPtr, GameRoom* RetRoomPtr)
{
	if (waitRoomCont.IsEmpty()) // Create!
	{
		CreateRoom(InUserPtr, RetRoomPtr);
		return true;
	}
	else // Join
	{
		JoinRoom(InUserPtr, RetRoomPtr);
		return false;
	}
}

