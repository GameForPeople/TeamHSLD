#include "../GameRoom/GameRoomManager.h"
#include "../IOCPServer/SocketInfo.h"

GameRoomManager::GameRoomManager() noexcept
	: pWaitClassicModeRoom()
	, pWaitCasualModeRoom()
	, roomNum(0)
{
	pWaitClassicModeRoom.reset();
	pWaitCasualModeRoom.reset();
	::InitializeCriticalSection(&csRoom);
}

GameRoomManager::~GameRoomManager()
{
	pWaitClassicModeRoom.reset();
	pWaitCasualModeRoom.reset();
	::DeleteCriticalSection(&csRoom);
}

bool GameRoomManager::RandomMatchingProcess(const shared_ptr<UserData>& pInUser, const bool isClientDemandClassicMode)
{
	EnterCriticalSection(&csRoom);
	//----------------------------

	// 있는 없든, 항상 리셋 ( 호스트와 게스트가 동일한 경우 방지
	pInUser->GetSocketInfo()->pRoomIter.reset();

	if (isClientDemandClassicMode)
	{
		if (pWaitClassicModeRoom.expired()) // Create!
		{
			_CreateRoom(pInUser, isClientDemandClassicMode);

			//----------------------------
			LeaveCriticalSection(&csRoom);

			++roomNum;		//atomic
			return true;
		}
		else // Join
		{
			_JoinRoom(pInUser, isClientDemandClassicMode);

			//----------------------------
			LeaveCriticalSection(&csRoom);
			return false;
		}
	}
	else /* if (isClientDemandClassicMode == false) */
	{
		if (pWaitCasualModeRoom.expired()) // Create!
		{
			_CreateRoom(pInUser, isClientDemandClassicMode);

			//----------------------------
			LeaveCriticalSection(&csRoom);

			++roomNum;		//atomic
			return true;
		}
		else // Join
		{
			_JoinRoom(pInUser, isClientDemandClassicMode);

			//----------------------------
			LeaveCriticalSection(&csRoom);
			return false;
		}
	}
}


void GameRoomManager::_CreateRoom(const shared_ptr<UserData>& pClient, const bool isClientDemandClassicMode)
{
	pClient->GetSocketInfo()->pRoomIter = make_shared<GameRoom>(pClient, isClientDemandClassicMode, false);

	if(isClientDemandClassicMode) pWaitClassicModeRoom = pClient->GetSocketInfo()->pRoomIter;
	else /*if (!isClientDemandClassicMode)*/  pWaitCasualModeRoom = pClient->GetSocketInfo()->pRoomIter;
}

void GameRoomManager::_JoinRoom(const shared_ptr<UserData>& pClient, const bool isClientDemandClassicMode)
{
	// Push -> Pop을 먼저 하고 (criticalSection을 걸고) 빼고 넣은 방에다가, 그 후 설정까지 끝내고 방 바꿈.
	
	if (isClientDemandClassicMode)			 pClient->GetSocketInfo()->pRoomIter = pWaitClassicModeRoom.lock();
	else /*if (!isClientDemandClassicMode)*/ pClient->GetSocketInfo()->pRoomIter = pWaitCasualModeRoom.lock();

	pClient->GetSocketInfo()->pRoomIter->JoinRoom(pClient);

	if (isClientDemandClassicMode)				pWaitClassicModeRoom.reset();
	else /*if (!isClientDemandClassicMode)*/	pWaitCasualModeRoom.reset();
}

bool GameRoomManager::HostCancelWaiting(SocketInfo* pClient)
{
	EnterCriticalSection(&csRoom);

	if (pClient->pRoomIter->roomState == ROOM_STATE::ROOM_STATE_SOLO)
	{
		if (pClient->pRoomIter->GetIsClassicMode())
		{
			pWaitClassicModeRoom.reset();
		}
		else
		{
			pWaitCasualModeRoom.reset();
		}

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



