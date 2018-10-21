#include "../SceneServer/RoomScene.h"

SCENE_NETWORK_MANAGER::RoomScene::RoomScene() : BaseScene(), PERMIT_ENEMY_CHARACTER(Protocol::PERMIT_ENEMY_CHARACTER)
{

}

void SCENE_NETWORK_MANAGER::RoomScene::ProcessData(const int& InRecvType, SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{
	DemandEnemyCharacterIndex(ptr, InRoomData, InUserData);
}

void SCENE_NETWORK_MANAGER::RoomScene::DemandEnemyCharacterIndex(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{
	InRoomData.SetCharacterIndex(ptr->roomIndex, ptr->isHost, (int&)ptr->buf[4]);

	//ptr->dataBuffer = new PermitEnemyCharacterStruct(roomData.GetEnemyCharacterIndex(ptr->roomIndex, ptr->isHost));
	
	memcpy(ptr->buf, (char*)&PERMIT_ENEMY_CHARACTER, sizeof(int));
	memcpy(ptr->buf + 4, (char*)(InRoomData.GetEnemyCharacterIndex(ptr->roomIndex, ptr->isHost)), sizeof(int));
	
	ptr->dataSize = 8;
}