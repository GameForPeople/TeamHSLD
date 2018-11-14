#include "../_5_InGameScene/InGameScene.h"

#include "../../IOCPServer/SocketInfo.h"

#include "../../GameRoom/GameRoomManager.h"
#include "../../UserData/UserDataManager.h"

SCENE_NETWORK_MANAGER::InGameScene::InGameScene(bool* InIsSaveOn) : BaseScene(), IsSaveOn(InIsSaveOn)
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

void SCENE_NETWORK_MANAGER::InGameScene::ProcessData(const int& InRecvType, SocketInfo* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{
	ProcessRecv(InRecvType, ptr, InRoomData, InUserData);

	//ProcessSend(InRoomData.GetDataProtocol(ptr->roomIndex, ptr->isHost), ptr, InRoomData, InUserData);
	ProcessSend(ptr->pRoomIter->GetDataProtocol(ptr->isHost), ptr, InRoomData, InUserData);
}

void SCENE_NETWORK_MANAGER::InGameScene::ProcessRecv(const int& InRecvType, SocketInfo* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{
	//if (InRecvType != 500)
	//	std::cout << "InRecvType : " << InRecvType << "\n";

	(this->*RecvFunctions[InRecvType - 500])(ptr, InRoomData, InUserData);
}


void SCENE_NETWORK_MANAGER::InGameScene::RecvGameState(SocketInfo* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{

}

void SCENE_NETWORK_MANAGER::InGameScene::RecvChangeTurn(SocketInfo* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{
	//InRoomData.SetDataProtocol(ptr->roomIndex, ptr->isHost, NOTIFY_CHANGE_TURN);
	ptr->pRoomIter->SetDataProtocol(ptr->isHost, NOTIFY_CHANGE_TURN);
}

void SCENE_NETWORK_MANAGER::InGameScene::RecvDiceValue(SocketInfo* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{
	//InRoomData.SetDataBuffer(ptr->roomIndex, ptr->isHost, ptr->buf + 4, sizeof(int));
	//InRoomData.SetDataProtocol(ptr->roomIndex, ptr->isHost, NOTIFY_DICE_VALUE);
	ptr->pRoomIter->SetDataBuffer(ptr->isHost, ptr->buf + 4, sizeof(int));
	ptr->pRoomIter->SetDataProtocol(ptr->isHost, NOTIFY_DICE_VALUE);
}

void SCENE_NETWORK_MANAGER::InGameScene::RecvTerrainType(SocketInfo* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{
	//InRoomData.SetDataBuffer(ptr->roomIndex, ptr->isHost, ptr->buf + 4, sizeof(int));
	//InRoomData.SetDataProtocol(ptr->roomIndex, ptr->isHost, NOTIFY_TERRAIN_TYPE);
	ptr->pRoomIter->SetDataBuffer(ptr->isHost, ptr->buf + 4, sizeof(int));
	ptr->pRoomIter->SetDataProtocol(ptr->isHost, NOTIFY_TERRAIN_TYPE);
}

void SCENE_NETWORK_MANAGER::InGameScene::RecvTerrainIndexs(SocketInfo* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{
	//InRoomData.SetDataBuffer(ptr->roomIndex, ptr->isHost, ptr->buf + 4, 76);
	//InRoomData.SetDataProtocol(ptr->roomIndex, ptr->isHost, NOTIFY_TERRAIN_INDEXS);
	ptr->pRoomIter->SetDataBuffer(ptr->isHost, ptr->buf + 4, 76);
	ptr->pRoomIter->SetDataProtocol(ptr->isHost, NOTIFY_TERRAIN_INDEXS);
}

void SCENE_NETWORK_MANAGER::InGameScene::RecvEventcardIndex(SocketInfo* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{
	//InRoomData.SetDataBuffer(ptr->roomIndex, ptr->isHost, ptr->buf + 4, sizeof(int));
	//InRoomData.SetDataProtocol(ptr->roomIndex, ptr->isHost, NOTIFY_EVENTCARD_INDEX);
	ptr->pRoomIter->SetDataBuffer(ptr->isHost, ptr->buf + 4, sizeof(int));
	ptr->pRoomIter->SetDataProtocol(ptr->isHost, NOTIFY_EVENTCARD_INDEX);
}

void SCENE_NETWORK_MANAGER::InGameScene::RecvNetworkExecption(SocketInfo* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{

}

void SCENE_NETWORK_MANAGER::InGameScene::RecvGameReady(SocketInfo* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{
	// 모든 게임에 대한 내 클라이언트의 준비가 끝나면, VOID_GAME_STATE으로 변경함 (최초 NOTIFY_GAME_READY 상태임.)
	ptr->pRoomIter->SetDataProtocol(ptr->isHost, VOID_GAME_STATE);

	//----------------------------------- DEV_59 - 1
	// 하나라도 Wait 상태되면, 굳이 
	ptr->pRoomIter->DeleteDynamicData();
	//-----------------------------------
}

// send Functions

void SCENE_NETWORK_MANAGER::InGameScene::ProcessSend(const int InSendType, SocketInfo* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{
	memcpy(ptr->buf, (char*)&InSendType, sizeof(int));
	
	//if(InSendType != 500)
	//	std::cout << "InSendType : " << InSendType << "\n";

	(this->*SendFunctions[InSendType - 500])(ptr, InRoomData, InUserData);
}

void SCENE_NETWORK_MANAGER::InGameScene::SendGameState(SocketInfo* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{
	// 이거를 최초로 받으면, 두 클라 모두 GameReady 상태가 된 것으로 판단.

	ptr->dataSize = 4;
}

void SCENE_NETWORK_MANAGER::InGameScene::SendChangeTurn(SocketInfo* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{
	//InRoomData.SetDataProtocol(ptr->roomIndex, !(ptr->isHost), VOID_GAME_STATE);

	ptr->pRoomIter->SetDataProtocol(!(ptr->isHost), VOID_GAME_STATE);
	ptr->dataSize = 4;
}

void SCENE_NETWORK_MANAGER::InGameScene::SendDiceValue(SocketInfo* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{
	//InRoomData.GetDataBuffer(ptr->roomIndex, ptr->isHost, ptr->buf + 4, sizeof(int));
	//InRoomData.SetDataProtocol(ptr->roomIndex, !(ptr->isHost), VOID_GAME_STATE);

	ptr->pRoomIter->GetDataBuffer(ptr->isHost, ptr->buf + 4, sizeof(int));
	ptr->pRoomIter->SetDataProtocol(!(ptr->isHost), VOID_GAME_STATE);

	ptr->dataSize = 8;
}

void SCENE_NETWORK_MANAGER::InGameScene::SendTerrainType(SocketInfo* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{
	//InRoomData.GetDataBuffer(ptr->roomIndex, ptr->isHost, ptr->buf + 4, sizeof(int));
	//InRoomData.SetDataProtocol(ptr->roomIndex, !(ptr->isHost), VOID_GAME_STATE);

	ptr->pRoomIter->GetDataBuffer(ptr->isHost, ptr->buf + 4, sizeof(int));
	ptr->pRoomIter->SetDataProtocol(!(ptr->isHost), VOID_GAME_STATE);

	ptr->dataSize = 8;
}

void SCENE_NETWORK_MANAGER::InGameScene::SendTerrainIndexs(SocketInfo* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{
	//InRoomData.GetDataBuffer(ptr->roomIndex, ptr->isHost, ptr->buf + 4, 76);
	//InRoomData.SetDataProtocol(ptr->roomIndex, !(ptr->isHost), VOID_GAME_STATE);
	
	ptr->pRoomIter->GetDataBuffer(ptr->isHost, ptr->buf + 4, 76);
	ptr->pRoomIter->SetDataProtocol(!(ptr->isHost), VOID_GAME_STATE);

	ptr->dataSize = 76;
}

void SCENE_NETWORK_MANAGER::InGameScene::SendEventcardIndex(SocketInfo* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{
	//InRoomData.GetDataBuffer(ptr->roomIndex, ptr->isHost, ptr->buf + 4, sizeof(int));
	//InRoomData.SetDataProtocol(ptr->roomIndex, !(ptr->isHost), VOID_GAME_STATE);

	ptr->pRoomIter->GetDataBuffer(ptr->isHost, ptr->buf + 4, sizeof(int));
	ptr->pRoomIter->SetDataProtocol(!(ptr->isHost), VOID_GAME_STATE);

	ptr->dataSize = 8;
}

void SCENE_NETWORK_MANAGER::InGameScene::SendNetworkExecption(SocketInfo* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{
	// 아마 네트워크 익셉션을 상대 클라이언트에게 직접 보낼 일은 딤져도 없음. -> 닥쳐 있을거야
//#ifdef _DEBUG
//	std::cout << "!! 그럴일이 없는데, 인 게임씬의 SendNetworkExecption가 호출되었습니다. 확인해주세요. " << "\n" ;
//#endif
	InUserData.SetGameResult(ptr->pUserNode, false);

	InRoomData.DestroyRoom(ptr);

	ptr->dataSize = 4;

	//*IsSaveOn = true;
}

void SCENE_NETWORK_MANAGER::InGameScene::SendGameReady(SocketInfo* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{
	// 이거 받으면, 507이니, 상대방이 아직 GameReady 상태가 안됨.
	ptr->dataSize = 4;
}