#include "../_4_RoomScene/RoomScene.h"

#include "../../IOCPServer/SocketInfo.h"
#include "../../IOCPServer/UDPManager.h"

SCENE_NETWORK_MANAGER::RoomScene::RoomScene() : BaseScene(), PERMIT_ENEMY_CHARACTER(Protocol::PERMIT_ENEMY_CHARACTER)
{

}

void SCENE_NETWORK_MANAGER::RoomScene::ProcessData(const int& InRecvType, SocketInfo* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData, UDPManager& InUDPManager)
{
	DemandEnemyCharacterIndex(ptr, InRoomData, InUserData);
}

void SCENE_NETWORK_MANAGER::RoomScene::DemandEnemyCharacterIndex(SocketInfo* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{
	//InRoomData.SetCharacterIndex(ptr->roomIndex, ptr->isHost, (int&)ptr->buf[4]);
	ptr->pRoomIter->SetCharacterIndex(ptr->isHost, (int&)ptr->buf[4]);


	//ptr->dataBuffer = new PermitEnemyCharacterStruct(roomData.GetEnemyCharacterIndex(ptr->roomIndex, ptr->isHost));
	int enemyIndexBuffer = (ptr->pRoomIter->GetEnemyCharacterIndex(ptr->isHost));

	memcpy(ptr->buf, (char*)&PERMIT_ENEMY_CHARACTER, sizeof(int));
	memcpy(ptr->buf + 4, (char*)&enemyIndexBuffer, sizeof(int));
	
	ptr->dataSize = 8;
}