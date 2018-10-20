#include "../SceneServer/TitleScene.h"

SCENE_NETWORK_MANAGER::TitleScene::TitleScene() : BaseScene(SCENE_PROTOCOL_START_INDEX::TITLE_SCENE)
{

}

void SCENE_NETWORK_MANAGER::TitleScene::ProcessRecv(const int& InRecvType, SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{
	RecvFunctions[InRecvType - sceneProtocolStartIndex](ptr, InRoomData, InUserData);
}

void SCENE_NETWORK_MANAGER::TitleScene::ProcessSend(const int& InSendType, SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{
	memcpy(ptr->buf, (char*)&InSendType, sizeof(int));

	SendFunctions[InSendType - sceneProtocolStartIndex](ptr, InRoomData, InUserData);
}