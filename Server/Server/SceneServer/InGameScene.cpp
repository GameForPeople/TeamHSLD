#include "../SceneServer/InGameScene.h"

INGAME_SCENE::InGameScene::InGameScene() : BaseScene(SCENE_PROTOCOL_START_INDEX::INGAMESCENE)
{
	//functionPointers.emplace_back(FunctionPointer(DemandGameState));
	RecvFunctions[0] = RecvGameState;
	RecvFunctions[1] = RecvChangeTurn;
	RecvFunctions[2] = RecvDiceValue;
	RecvFunctions[3] = RecvTerrainType;
	RecvFunctions[4] = RecvTerrainIndexs;
	RecvFunctions[5] = RecvEventcardIndex;

	SendFunctions[0] = SendGameState;
	SendFunctions[1] = SendChangeTurn;
	SendFunctions[2] = SendDiceValue;
	SendFunctions[3] = SendTerrainType;
	SendFunctions[4] = SendTerrainIndexs;
	SendFunctions[5] = SendEventcardIndex;
}

void INGAME_SCENE::InGameScene::ProcessRecv(const int& InRecvType, SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{
	RecvFunctions[InRecvType - sceneProtocolStartIndex](ptr, InRoomData, InUserData);
}

void INGAME_SCENE::RecvGameState(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{

}

void INGAME_SCENE::RecvChangeTurn(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{
	InRoomData.SetDataProtocol(ptr->roomIndex, ptr->isHost, NOTIFY_CHANGE_TURN);
}

void INGAME_SCENE::RecvDiceValue(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{
	InRoomData.SetDataProtocol(ptr->roomIndex, ptr->isHost, NOTIFY_DICE_VALUE);
	InRoomData.SetDataBuffer(ptr->roomIndex, ptr->isHost, ptr->buf + 4, sizeof(int));
}

void INGAME_SCENE::RecvTerrainType(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{
	InRoomData.SetDataProtocol(ptr->roomIndex, ptr->isHost, NOTIFY_TERRAIN_TYPE);
	InRoomData.SetDataBuffer(ptr->roomIndex, ptr->isHost, ptr->buf + 4, sizeof(int));
}

void INGAME_SCENE::RecvTerrainIndexs(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{
	InRoomData.SetDataProtocol(ptr->roomIndex, ptr->isHost, NOTIFY_TERRAIN_INDEXS);
	InRoomData.SetDataBuffer(ptr->roomIndex, ptr->isHost, ptr->buf + 4, (int&)(ptr->buf[4]) + 4);
}

void INGAME_SCENE::RecvEventcardIndex(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{
	InRoomData.SetDataProtocol(ptr->roomIndex, ptr->isHost, NOTIFY_EVENTCARD_INDEX);
	InRoomData.SetDataBuffer(ptr->roomIndex, ptr->isHost, ptr->buf + 4, sizeof(int));
}

