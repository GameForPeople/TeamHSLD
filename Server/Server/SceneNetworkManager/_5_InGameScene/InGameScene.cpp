#include "../_5_InGameScene/InGameScene.h"

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

void SCENE_NETWORK_MANAGER::InGameScene::ProcessData(const int& InRecvType, SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{
	ProcessRecv(InRecvType, ptr, InRoomData, InUserData);

	ProcessSend(InRoomData.GetDataProtocol(ptr->roomIndex, ptr->isHost), ptr, InRoomData, InUserData);
}

void SCENE_NETWORK_MANAGER::InGameScene::ProcessRecv(const int& InRecvType, SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{
	//if (InRecvType != 500)
	//	std::cout << "InRecvType : " << InRecvType << "\n";

	(this->*RecvFunctions[InRecvType - 500])(ptr, InRoomData, InUserData);
}


void SCENE_NETWORK_MANAGER::InGameScene::RecvGameState(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{

}

void SCENE_NETWORK_MANAGER::InGameScene::RecvChangeTurn(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{
	InRoomData.SetDataProtocol(ptr->roomIndex, ptr->isHost, NOTIFY_CHANGE_TURN);
}

void SCENE_NETWORK_MANAGER::InGameScene::RecvDiceValue(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{
	InRoomData.SetDataBuffer(ptr->roomIndex, ptr->isHost, ptr->buf + 4, sizeof(int));
	InRoomData.SetDataProtocol(ptr->roomIndex, ptr->isHost, NOTIFY_DICE_VALUE);
}

void SCENE_NETWORK_MANAGER::InGameScene::RecvTerrainType(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{
	InRoomData.SetDataBuffer(ptr->roomIndex, ptr->isHost, ptr->buf + 4, sizeof(int));
	InRoomData.SetDataProtocol(ptr->roomIndex, ptr->isHost, NOTIFY_TERRAIN_TYPE);
}

void SCENE_NETWORK_MANAGER::InGameScene::RecvTerrainIndexs(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{
	InRoomData.SetDataBuffer(ptr->roomIndex, ptr->isHost, ptr->buf + 4, 76);
	InRoomData.SetDataProtocol(ptr->roomIndex, ptr->isHost, NOTIFY_TERRAIN_INDEXS);
}

void SCENE_NETWORK_MANAGER::InGameScene::RecvEventcardIndex(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{
	InRoomData.SetDataBuffer(ptr->roomIndex, ptr->isHost, ptr->buf + 4, sizeof(int));
	InRoomData.SetDataProtocol(ptr->roomIndex, ptr->isHost, NOTIFY_EVENTCARD_INDEX);

}

void SCENE_NETWORK_MANAGER::InGameScene::RecvNetworkExecption(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{

}



// send Functions

void SCENE_NETWORK_MANAGER::InGameScene::ProcessSend(const int InSendType, SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{
	memcpy(ptr->buf, (char*)&InSendType, sizeof(int));
	
	//if(InSendType != 500)
	//	std::cout << "InSendType : " << InSendType << "\n";

	(this->*SendFunctions[InSendType - 500])(ptr, InRoomData, InUserData);
}

void SCENE_NETWORK_MANAGER::InGameScene::SendGameState(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{
	ptr->dataSize = 4;
}

void SCENE_NETWORK_MANAGER::InGameScene::SendChangeTurn(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{
	InRoomData.SetDataProtocol(ptr->roomIndex, !(ptr->isHost), VOID_GAME_STATE);
	ptr->dataSize = 4;

}

void SCENE_NETWORK_MANAGER::InGameScene::SendDiceValue(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{
	InRoomData.GetDataBuffer(ptr->roomIndex, ptr->isHost, ptr->buf + 4, sizeof(int));
	InRoomData.SetDataProtocol(ptr->roomIndex, !(ptr->isHost), VOID_GAME_STATE);

	ptr->dataSize = 8;
}

void SCENE_NETWORK_MANAGER::InGameScene::SendTerrainType(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{
	InRoomData.GetDataBuffer(ptr->roomIndex, ptr->isHost, ptr->buf + 4, sizeof(int));
	InRoomData.SetDataProtocol(ptr->roomIndex, !(ptr->isHost), VOID_GAME_STATE);

	ptr->dataSize = 8;

}

void SCENE_NETWORK_MANAGER::InGameScene::SendTerrainIndexs(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{
	InRoomData.GetDataBuffer(ptr->roomIndex, ptr->isHost, ptr->buf + 4, 76);
	InRoomData.SetDataProtocol(ptr->roomIndex, !(ptr->isHost), VOID_GAME_STATE);
	ptr->dataSize = 76;
}

void SCENE_NETWORK_MANAGER::InGameScene::SendEventcardIndex(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{
	InRoomData.GetDataBuffer(ptr->roomIndex, ptr->isHost, ptr->buf + 4, sizeof(int));
	InRoomData.SetDataProtocol(ptr->roomIndex, !(ptr->isHost), VOID_GAME_STATE);

	ptr->dataSize = 8;
}

void SCENE_NETWORK_MANAGER::InGameScene::SendNetworkExecption(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{
	// �Ƹ� ��Ʈ��ũ �ͼ����� ��� Ŭ���̾�Ʈ���� ���� ���� ���� ������ ����. -> ���� �����ž�
//#ifdef _DEBUG
//	std::cout << "!! �׷����� ���µ�, �� ���Ӿ��� SendNetworkExecption�� ȣ��Ǿ����ϴ�. Ȯ�����ּ���. " << "\n" ;
//#endif
	
	// ������ ���� �� ������ ��, �̹� ��Ʈ��ũ ���� ���ܿ���, �ش� ���ӿ� ���� ����� ó������. (������� 1��, �մ� ��� 1��)
	// ���⼭�� �游 �����ְ�, �ش� Ŭ���̾�Ʈ����, MainUIScene������ �̵��� ���.

	InRoomData.ExitRoom(ptr->roomIndex);
	ptr->roomIndex = -1;

	ptr->dataSize = 4;

	*IsSaveOn = true;
}
