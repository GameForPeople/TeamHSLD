#include "../GameRoom/GameRoomManager.h"
#include "../IOCPServer/SocketInfo.h"

GameRoomManager::GameRoomManager() noexcept
	: pWaitRoom(), roomNum(0)
{
	pWaitRoom.reset();
	::InitializeCriticalSection(&csRoom);
}

GameRoomManager::~GameRoomManager()
{
	pWaitRoom.reset();
	::DeleteCriticalSection(&csRoom);
}

bool GameRoomManager::RandomMatchingProcess(const shared_ptr<UserData>& pInUser)
{
	EnterCriticalSection(&csRoom);
	//----------------------------

	if (pWaitRoom.expired()) // Create!
	{
		_CreateRoom(pInUser);

		//----------------------------
		LeaveCriticalSection(&csRoom);

		++roomNum;		//atomic
		return true;
	}
	else // Join
	{
		_JoinRoom(pInUser);

		//----------------------------
		LeaveCriticalSection(&csRoom);
		return false;
	}
}


void GameRoomManager::_CreateRoom(const shared_ptr<UserData>& pClient)
{
	pClient->GetSocketInfo()->pRoomIter = make_shared<GameRoom>(pClient);
	pWaitRoom = pClient->GetSocketInfo()->pRoomIter;
}

void GameRoomManager::_JoinRoom(const shared_ptr<UserData>& pClient)
{
	// Push -> Pop을 먼저 하고 (criticalSection을 걸고) 빼고 넣은 방에다가, 그 후 설정까지 끝내고 방 바꿈.
	pClient->GetSocketInfo()->pRoomIter = pWaitRoom.lock();
	pClient->GetSocketInfo()->pRoomIter->JoinRoom(pClient);
	pWaitRoom.reset();
}

bool GameRoomManager::HostCancelWaiting(SocketInfo* pClient)
{
	EnterCriticalSection(&csRoom);

	if (pClient->pRoomIter->roomState == ROOM_STATE::ROOM_STATE_SOLO)
	{
		pClient->pRoomIter.reset();

		LeaveCriticalSection(&csRoom);
		return true;
	}
	else // ROOM_STATE::ROOM_STATE_PLAY
	{
		LeaveCriticalSection(&csRoom);
		return false;
	}
}

void GameRoomManager::_DestroyRoom(SocketInfo* pClient)
{
	pClient->pRoomIter.reset();
}



