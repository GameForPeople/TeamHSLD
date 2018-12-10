#include "../GameRoom/GameRoomManager.h"
#include "../IOCPServer/SocketInfo.h"


GameRoom* GameRoomManager::_CreateRoom(const shared_ptr<UserData>& pInUserNode)
{
	return waitRoomCont.Create(pInUserNode);
}

GameRoom* GameRoomManager::_JoinRoom(const shared_ptr<UserData>& pInUserNode, GameRoom* pRetRoom)
{
	// Push -> Pop을 먼저 하고 (criticalSection을 걸고) 빼고 넣은 방에다가, 그 후 설정까지 끝내고 방 바꿈.

	pRetRoom = waitRoomCont.GetRootNode(pRetRoom);

	playRoomCont.Push(pRetRoom);

	pRetRoom->JoinRoom(pInUserNode);

	return pRetRoom;
}

void GameRoomManager::DestroyRoom(SocketInfo* pClient)
{
	if (pClient->pRoomIter->GetGamePlay())
	{
		playRoomCont.Pop(pClient->pRoomIter);
	}
	else
	{
		waitRoomCont.Pop(pClient->pRoomIter);
	}

	delete (pClient->pRoomIter);

	pClient->pRoomIter = nullptr;
}

bool GameRoomManager::CancelWait(SocketInfo* pClient)
{
	if (pClient->pRoomIter->GetGamePlay())
	{
		return false;
	}
	else
	{
		DestroyRoom(pClient);
		return true;
	}
}

GameRoom* GameRoomManager::RandomMatchingProcess(const shared_ptr<UserData>& pInUser, GameRoom* pRetRoom, bool& RetBoolBuffer)
{
	if (waitRoomCont.IsEmpty()) // Create!
	{
		RetBoolBuffer = true;
		pRetRoom = _CreateRoom(pInUser);
	}
	else // Join
	{
		RetBoolBuffer = false;
		pRetRoom = _JoinRoom(pInUser, pRetRoom);
	}
	
	return pRetRoom;
}

