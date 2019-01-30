#include "../_0_TitleScene/TitleScene.h"

SCENE_NETWORK_MANAGER::TitleScene::TitleScene(GameRoomManager* pInRoomData, UserDataManager* pInUserData, UDPManager* pInUDPManager) 
	: BaseScene(pInRoomData, pInUserData, pInUDPManager)
	, CONST_ANSWER_ANNOUNCEMENT(Protocol::ANSWER_ANNOUNCEMENT)
{
}

void SCENE_NETWORK_MANAGER::TitleScene::ProcessData(const int InRecvType, SocketInfo* pClient)
{
	if (InRecvType == Protocol::DEMAND_ANNOUNCEMENT)
		_RecvDemandAnnouncement(pClient);
}

void SCENE_NETWORK_MANAGER::TitleScene::_RecvDemandAnnouncement(SocketInfo* pClient)
{
	const wstring strBuf = pUDPManager->GetAnnounceString();
	int sizeBuf = strBuf.size() * 2;

	memcpy(pClient->buf, reinterpret_cast<const char*>(&CONST_ANSWER_ANNOUNCEMENT), 4);
	memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&sizeBuf), 4);
	memcpy(pClient->buf + 8, strBuf.data(), sizeBuf);
	
	pClient->dataSize = 8 + sizeBuf;
}

//void SCENE_NETWORK_MANAGER::TitleScene::ProcessRecv(const int& InRecvType, SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
//{
//	RecvFunctions[InRecvType - sceneProtocolStartIndex](ptr, InRoomData, InUserData);
//}
//
//void SCENE_NETWORK_MANAGER::TitleScene::ProcessSend(const int& InSendType, SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
//{
//	memcpy(ptr.buf, (char*)&InSendType, sizeof(int));
//
//	SendFunctions[InSendType - sceneProtocolStartIndex](ptr, InRoomData, InUserData);
//}