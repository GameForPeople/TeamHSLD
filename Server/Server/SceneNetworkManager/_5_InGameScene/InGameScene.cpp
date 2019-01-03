#include "../_5_InGameScene/InGameScene.h"

#include "../../IOCPServer/SocketInfo.h"

#include "../../GameRoom/GameRoomManager.h"
#include "../../UserData/UserDataManager.h"
#include "../../IOCPServer/UDPManager.h"

SCENE_NETWORK_MANAGER::InGameScene::InGameScene(GameRoomManager* pInRoomData, UserDataManager* pInUserData, UDPManager* pInUDPManager) 
	: BaseScene(pInRoomData, pInUserData, pInUDPManager)
{
	//functionPointers.emplace_back(FunctionPointer(DemandGameState));
	RecvFunctions[0] = &InGameScene::RecvGameState;
	RecvFunctions[1] = &InGameScene::RecvChangeTurn;
	RecvFunctions[2] = &InGameScene::RecvDiceValue;
	RecvFunctions[3] = &InGameScene::RecvTerrainType;
	RecvFunctions[4] = &InGameScene::RecvTerrainIndexs;
	RecvFunctions[5] = &InGameScene::RecvEventcardIndex;
	RecvFunctions[6] = &InGameScene::RecvNetworkExecption;
	RecvFunctions[7] = &InGameScene::RecvGameReady;

	SendFunctions[0] = &InGameScene::SendGameState;
	SendFunctions[1] = &InGameScene::SendChangeTurn;
	SendFunctions[2] = &InGameScene::SendDiceValue;
	SendFunctions[3] = &InGameScene::SendTerrainType;
	SendFunctions[4] = &InGameScene::SendTerrainIndexs;
	SendFunctions[5] = &InGameScene::SendEventcardIndex;
	SendFunctions[6] = &InGameScene::SendNetworkExecption;
	SendFunctions[7] = &InGameScene::SendGameReady;
}

void SCENE_NETWORK_MANAGER::InGameScene::ProcessData(const int InRecvType, SocketInfo* pClient)
{
	ProcessRecv(InRecvType, pClient);

	//ProcessSend(InRoomData.GetDataProtocol(ptr->roomIndex, ptr->isHost), ptr, InRoomData, InUserData);
	ProcessSend(pClient->pRoomIter->GetDataProtocol(pClient->isHost), pClient);
}

void SCENE_NETWORK_MANAGER::InGameScene::ProcessRecv(const int InRecvType, SocketInfo* pClient)
{
	//if (InRecvType != 500)
	//	std::cout << "InRecvType : " << InRecvType << "\n";

	(this->*RecvFunctions[InRecvType - 500])(pClient);
}


void SCENE_NETWORK_MANAGER::InGameScene::RecvGameState(SocketInfo* pClient)
{

}

void SCENE_NETWORK_MANAGER::InGameScene::RecvChangeTurn(SocketInfo* pClient)
{
	//InRoomData.SetDataProtocol(ptr->roomIndex, ptr->isHost, NOTIFY_CHANGE_TURN);
	pClient->pRoomIter->SetDataProtocol(pClient->isHost, NOTIFY_CHANGE_TURN);
}

void SCENE_NETWORK_MANAGER::InGameScene::RecvDiceValue(SocketInfo* pClient)
{
	//InRoomData.SetDataBuffer(ptr->roomIndex, ptr->isHost, ptr->buf + 4, sizeof(int));
	//InRoomData.SetDataProtocol(ptr->roomIndex, ptr->isHost, NOTIFY_DICE_VALUE);
	pClient->pRoomIter->SetDataBuffer(pClient->isHost, pClient->buf + 4, sizeof(int));
	pClient->pRoomIter->SetDataProtocol(pClient->isHost, NOTIFY_DICE_VALUE);
}

void SCENE_NETWORK_MANAGER::InGameScene::RecvTerrainType(SocketInfo* pClient)
{
	//InRoomData.SetDataBuffer(ptr->roomIndex, ptr->isHost, ptr->buf + 4, sizeof(int));
	//InRoomData.SetDataProtocol(ptr->roomIndex, ptr->isHost, NOTIFY_TERRAIN_TYPE);
	pClient->pRoomIter->SetDataBuffer(pClient->isHost, pClient->buf + 4, sizeof(int));
	pClient->pRoomIter->SetDataProtocol(pClient->isHost, NOTIFY_TERRAIN_TYPE);
}

void SCENE_NETWORK_MANAGER::InGameScene::RecvTerrainIndexs(SocketInfo* pClient)
{
	//InRoomData.SetDataBuffer(ptr->roomIndex, ptr->isHost, ptr->buf + 4, 76);
	//InRoomData.SetDataProtocol(ptr->roomIndex, ptr->isHost, NOTIFY_TERRAIN_INDEXS);
	pClient->pRoomIter->SetDataBuffer(pClient->isHost, pClient->buf + 4, 76);
	pClient->pRoomIter->SetDataProtocol(pClient->isHost, NOTIFY_TERRAIN_INDEXS);
}

void SCENE_NETWORK_MANAGER::InGameScene::RecvEventcardIndex(SocketInfo* pClient)
{
	//InRoomData.SetDataBuffer(ptr->roomIndex, ptr->isHost, ptr->buf + 4, sizeof(int));
	//InRoomData.SetDataProtocol(ptr->roomIndex, ptr->isHost, NOTIFY_EVENTCARD_INDEX);
	pClient->pRoomIter->SetDataBuffer(pClient->isHost, pClient->buf + 4, sizeof(int));
	pClient->pRoomIter->SetDataProtocol(pClient->isHost, NOTIFY_EVENTCARD_INDEX);
}

void SCENE_NETWORK_MANAGER::InGameScene::RecvNetworkExecption(SocketInfo* pClient)
{

}

void SCENE_NETWORK_MANAGER::InGameScene::RecvGameReady(SocketInfo* pClient)
{
	// ��� ���ӿ� ���� �� Ŭ���̾�Ʈ�� �غ� ������, VOID_GAME_STATE���� ������ (���� NOTIFY_GAME_READY ������.)
	pClient->pRoomIter->SetDataProtocol(pClient->isHost, VOID_GAME_STATE);

	//----------------------------------- DEV_59 - 1
	// �ϳ��� Wait ���µǸ�, ���� 
	pClient->pRoomIter->DeleteDynamicData();
	//-----------------------------------
}

// send Functions

void SCENE_NETWORK_MANAGER::InGameScene::ProcessSend(const int InSendType, SocketInfo* pClient)
{
	memcpy(pClient->buf, reinterpret_cast<const char*>(&InSendType), sizeof(int));
	
	//if(InSendType != 500)
	//	std::cout << "InSendType : " << InSendType << "\n";

	(this->*SendFunctions[InSendType - 500])(pClient);
}

void SCENE_NETWORK_MANAGER::InGameScene::SendGameState(SocketInfo* pClient)
{
	// �̰Ÿ� ���ʷ� ������, �� Ŭ�� ��� GameReady ���°� �� ������ �Ǵ�.

	pClient->dataSize = 4;
}

void SCENE_NETWORK_MANAGER::InGameScene::SendChangeTurn(SocketInfo* pClient)
{
	//InRoomData.SetDataProtocol(ptr->roomIndex, !(ptr->isHost), VOID_GAME_STATE);

	pClient->pRoomIter->SetDataProtocol(!(pClient->isHost), VOID_GAME_STATE);
	pClient->dataSize = 4;
}

void SCENE_NETWORK_MANAGER::InGameScene::SendDiceValue(SocketInfo* pClient)
{
	//InRoomData.GetDataBuffer(ptr->roomIndex, ptr->isHost, ptr->buf + 4, sizeof(int));
	//InRoomData.SetDataProtocol(ptr->roomIndex, !(ptr->isHost), VOID_GAME_STATE);

	pClient->pRoomIter->GetDataBuffer(pClient->isHost, pClient->buf + 4, sizeof(int));
	pClient->pRoomIter->SetDataProtocol(!(pClient->isHost), VOID_GAME_STATE);

	pClient->dataSize = 8;
}

void SCENE_NETWORK_MANAGER::InGameScene::SendTerrainType(SocketInfo* pClient)
{
	//InRoomData.GetDataBuffer(ptr->roomIndex, ptr->isHost, ptr->buf + 4, sizeof(int));
	//InRoomData.SetDataProtocol(ptr->roomIndex, !(ptr->isHost), VOID_GAME_STATE);

	pClient->pRoomIter->GetDataBuffer(pClient->isHost, pClient->buf + 4, sizeof(int));
	pClient->pRoomIter->SetDataProtocol(!(pClient->isHost), VOID_GAME_STATE);

	pClient->dataSize = 8;
}

void SCENE_NETWORK_MANAGER::InGameScene::SendTerrainIndexs(SocketInfo* pClient)
{
	//InRoomData.GetDataBuffer(ptr->roomIndex, ptr->isHost, ptr->buf + 4, 76);
	//InRoomData.SetDataProtocol(ptr->roomIndex, !(ptr->isHost), VOID_GAME_STATE);
	
	pClient->pRoomIter->GetDataBuffer(pClient->isHost, pClient->buf + 4, 76);
	pClient->pRoomIter->SetDataProtocol(!(pClient->isHost), VOID_GAME_STATE);

	pClient->dataSize = 76;
}

void SCENE_NETWORK_MANAGER::InGameScene::SendEventcardIndex(SocketInfo* pClient)
{
	//InRoomData.GetDataBuffer(ptr->roomIndex, ptr->isHost, ptr->buf + 4, sizeof(int));
	//InRoomData.SetDataProtocol(ptr->roomIndex, !(ptr->isHost), VOID_GAME_STATE);

	pClient->pRoomIter->GetDataBuffer(pClient->isHost, pClient->buf + 4, sizeof(int));
	pClient->pRoomIter->SetDataProtocol(!(pClient->isHost), VOID_GAME_STATE);

	pClient->dataSize = 8;
}

void SCENE_NETWORK_MANAGER::InGameScene::SendNetworkExecption(SocketInfo* pClient)
{
	// �Ƹ� ��Ʈ��ũ �ͼ����� ��� Ŭ���̾�Ʈ���� ���� ���� ���� ������ ����. -> ���� �����ž�
//#ifdef _DEBUG
//	std::cout << "!! �׷����� ���µ�, �� ���Ӿ��� SendNetworkExecption�� ȣ��Ǿ����ϴ�. Ȯ�����ּ���. " << "\n" ;
//#endif
	pUserData->SetGameResult(pClient->pUserNode, false);

	//pRoomData->DestroyRoom(pClient);
	pClient->pRoomIter.reset();

	pClient->dataSize = 4;

	//*IsSaveOn = true;
}

void SCENE_NETWORK_MANAGER::InGameScene::SendGameReady(SocketInfo* pClient)
{
	// �̰� ������, 507�̴�, ������ ���� GameReady ���°� �ȵ�.
	pClient->dataSize = 4;
}