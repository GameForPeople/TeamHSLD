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


	SendFunctions[0] = &InGameScene::SendGameState;
	SendFunctions[1] = &InGameScene::SendChangeTurn;
	SendFunctions[2] = &InGameScene::SendDiceValue;
	SendFunctions[3] = &InGameScene::SendTerrainType;
	SendFunctions[4] = &InGameScene::SendTerrainIndexs;
	SendFunctions[5] = &InGameScene::SendEventcardIndex;
	SendFunctions[6] = &InGameScene::SendNetworkExecption;
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
	
	// 상대방이 게임 중 나갔을 때, 이미 네트워크 소켓 예외에서, 해당 게임에 대한 결과를 처리해줌. (나간사람 1패, 잇던 사람 1승)
	// 여기서는 방만 없애주고, 해당 클라이언트에게, MainUIScene으로의 이등을 명령.

	//InRoomData.ExitRoom(ptr->roomIndex);
	InRoomData.DestroyRoom(ptr);

	//ptr->roomIndex = -1; // DestoryRoom에서 얼려줌..

	ptr->dataSize = 4;

	*IsSaveOn = true;
}
