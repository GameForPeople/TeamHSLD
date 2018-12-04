#include "../_4_RoomScene/RoomScene.h"

#include "../../IOCPServer/SocketInfo.h"
#include "../../IOCPServer/UDPManager.h"

SCENE_NETWORK_MANAGER::RoomScene::RoomScene(GameRoomManager* pInRoomData, UserDataManager* pInUserData, UDPManager* pInUDPManager) 
	: BaseScene(pInRoomData, pInUserData, pInUDPManager)
	, PERMIT_ENEMY_CHARACTER(Protocol::PERMIT_ENEMY_CHARACTER)
{

}

void SCENE_NETWORK_MANAGER::RoomScene::ProcessData(const int& InRecvType, SocketInfo* pClient)
{
	_DemandEnemyCharacterIndex(pClient);
}

void SCENE_NETWORK_MANAGER::RoomScene::_DemandEnemyCharacterIndex(SocketInfo* pClient)
{
	//InRoomData.SetCharacterIndex(ptr->roomIndex, ptr->isHost, (int&)ptr->buf[4]);
	pClient->pRoomIter->SetCharacterIndex(pClient->isHost, (int&)pClient->buf[4]);


	//ptr->dataBuffer = new PermitEnemyCharacterStruct(roomData.GetEnemyCharacterIndex(ptr->roomIndex, ptr->isHost));
	int enemyIndexBuffer = (pClient->pRoomIter->GetEnemyCharacterIndex(pClient->isHost));

	memcpy(pClient->buf, reinterpret_cast<const char*>(&PERMIT_ENEMY_CHARACTER), sizeof(int));
	memcpy(pClient->buf + 4, reinterpret_cast<char*>(&enemyIndexBuffer), sizeof(int));
	
	pClient->dataSize = 8;
}