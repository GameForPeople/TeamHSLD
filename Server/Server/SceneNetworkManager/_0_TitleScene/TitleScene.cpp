#include "../_0_TitleScene/TitleScene.h"

SCENE_NETWORK_MANAGER::TitleScene::TitleScene(GameRoomManager* pInRoomData, UserDataManager* pInUserData, UDPManager* pInUDPManager) 
	: BaseScene(pInRoomData, pInUserData, pInUDPManager)
	, CONST_ANSWER_ANNOUNCEMENT(Protocol::ANSWER_ANNOUNCEMENT)
	, CHAT_BLOCK_SIZE(10)
	, nowChatIndex(0)
{
	chatBlockArr.reserve(CHAT_BLOCK_SIZE);

	for (int i = 0; i < CHAT_BLOCK_SIZE; ++i) { chatBlockArr.emplace_back(); }
}

void SCENE_NETWORK_MANAGER::TitleScene::ProcessData(const int InRecvType, SocketInfo* pClient)
{
	if (InRecvType == Protocol::DEMAND_ANNOUNCEMENT)
		_RecvDemandAnnouncement(pClient);
	else if (InRecvType == Protocol::DEMAND_ADD_CHAT)
		_RecvDemandAddChat(pClient);
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

void SCENE_NETWORK_MANAGER::TitleScene::_RecvDemandAddChat(SocketInfo* pClient)
{

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