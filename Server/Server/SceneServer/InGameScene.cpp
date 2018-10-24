#include "../SceneServer/InGameScene.h"

SCENE_NETWORK_MANAGER::InGameScene::InGameScene() : BaseScene()
{
	//functionPointers.emplace_back(FunctionPointer(DemandGameState));
	RecvFunctions[0] = RecvGameState;
	RecvFunctions[1] = RecvChangeTurn;
	RecvFunctions[2] = RecvDiceValue;
	RecvFunctions[3] = RecvTerrainType;
	RecvFunctions[4] = RecvTerrainIndexs;
	RecvFunctions[5] = RecvEventcardIndex;
	RecvFunctions[6] = RecvNetworkExecption;


	SendFunctions[0] = SendGameState;
	SendFunctions[1] = SendChangeTurn;
	SendFunctions[2] = SendDiceValue;
	SendFunctions[3] = SendTerrainType;
	SendFunctions[4] = SendTerrainIndexs;
	SendFunctions[5] = SendEventcardIndex;
	SendFunctions[6] = SendNetworkExecption;
}

void SCENE_NETWORK_MANAGER::InGameScene::ProcessData(const int& InRecvType, SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{
	ProcessRecv(InRecvType, ptr, InRoomData, InUserData);

	ProcessSend(InRoomData.GetDataProtocol(ptr->roomIndex, ptr->isHost), ptr, InRoomData, InUserData);
}

void SCENE_NETWORK_MANAGER::InGameScene::ProcessRecv(const int& InRecvType, SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{
	//if (InRecvType != 500)
	//	std::cout << "InRecvType : " << InRecvType << "\n";

	RecvFunctions[InRecvType - 500](ptr, InRoomData, InUserData);
}


void SCENE_NETWORK_MANAGER::RecvGameState(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{

}

void SCENE_NETWORK_MANAGER::RecvChangeTurn(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{
	InRoomData.SetDataProtocol(ptr->roomIndex, ptr->isHost, NOTIFY_CHANGE_TURN);
}

void SCENE_NETWORK_MANAGER::RecvDiceValue(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{
	InRoomData.SetDataBuffer(ptr->roomIndex, ptr->isHost, ptr->buf + 4, sizeof(int));
	InRoomData.SetDataProtocol(ptr->roomIndex, ptr->isHost, NOTIFY_DICE_VALUE);
}

void SCENE_NETWORK_MANAGER::RecvTerrainType(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{
	InRoomData.SetDataBuffer(ptr->roomIndex, ptr->isHost, ptr->buf + 4, sizeof(int));
	InRoomData.SetDataProtocol(ptr->roomIndex, ptr->isHost, NOTIFY_TERRAIN_TYPE);
}

void SCENE_NETWORK_MANAGER::RecvTerrainIndexs(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{
	InRoomData.SetDataBuffer(ptr->roomIndex, ptr->isHost, ptr->buf + 4, 76);
	InRoomData.SetDataProtocol(ptr->roomIndex, ptr->isHost, NOTIFY_TERRAIN_INDEXS);
}

void SCENE_NETWORK_MANAGER::RecvEventcardIndex(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{
	InRoomData.SetDataBuffer(ptr->roomIndex, ptr->isHost, ptr->buf + 4, sizeof(int));
	InRoomData.SetDataProtocol(ptr->roomIndex, ptr->isHost, NOTIFY_EVENTCARD_INDEX);

}

void SCENE_NETWORK_MANAGER::RecvNetworkExecption(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{

}



// send Functions

void SCENE_NETWORK_MANAGER::InGameScene::ProcessSend(const int InSendType, SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{
	memcpy(ptr->buf, (char*)&InSendType, sizeof(int));
	
	//if(InSendType != 500)
	//	std::cout << "InSendType : " << InSendType << "\n";

	SendFunctions[InSendType - 500](ptr, InRoomData, InUserData);
}

void SCENE_NETWORK_MANAGER::SendGameState(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{
	ptr->dataSize = 4;
}

void SCENE_NETWORK_MANAGER::SendChangeTurn(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{
	InRoomData.SetDataProtocol(ptr->roomIndex, !(ptr->isHost), VOID_GAME_STATE);
	ptr->dataSize = 4;

}

void SCENE_NETWORK_MANAGER::SendDiceValue(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{
	InRoomData.GetDataBuffer(ptr->roomIndex, ptr->isHost, ptr->buf + 4, sizeof(int));
	InRoomData.SetDataProtocol(ptr->roomIndex, !(ptr->isHost), VOID_GAME_STATE);

	ptr->dataSize = 8;
}

void SCENE_NETWORK_MANAGER::SendTerrainType(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{
	InRoomData.GetDataBuffer(ptr->roomIndex, ptr->isHost, ptr->buf + 4, sizeof(int));
	InRoomData.SetDataProtocol(ptr->roomIndex, !(ptr->isHost), VOID_GAME_STATE);

	ptr->dataSize = 8;

}

void SCENE_NETWORK_MANAGER::SendTerrainIndexs(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{
	InRoomData.GetDataBuffer(ptr->roomIndex, ptr->isHost, ptr->buf + 4, 76);
	InRoomData.SetDataProtocol(ptr->roomIndex, !(ptr->isHost), VOID_GAME_STATE);
	ptr->dataSize = 76;
}

void SCENE_NETWORK_MANAGER::SendEventcardIndex(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{
	InRoomData.GetDataBuffer(ptr->roomIndex, ptr->isHost, ptr->buf + 4, sizeof(int));
	InRoomData.SetDataProtocol(ptr->roomIndex, !(ptr->isHost), VOID_GAME_STATE);

	ptr->dataSize = 8;
}

void SCENE_NETWORK_MANAGER::SendNetworkExecption(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{
	// 아마 네트워크 익셉션을 상대 클라이언트에게 직접 보낼 일은 딤져도 없음. -> 닥쳐 있을거야
//#ifdef _DEBUG
//	std::cout << "!! 그럴일이 없는데, 인 게임씬의 SendNetworkExecption가 호출되었습니다. 확인해주세요. " << "\n" ;
//#endif
	
	// 상대방이 게임 중 나갔을 때, 이미 네트워크 소켓 예외에서, 해당 게임에 대한 결과를 처리해줌. (나간사람 1패, 잇던 사람 1승)
	// 여기서는 방만 없애주고, 해당 클라이언트에게, MainUIScene으로의 이등을 명령.

	InRoomData.ExitRoom(ptr->roomIndex);
	ptr->roomIndex = -1;

	ptr->dataSize = 4;
}
