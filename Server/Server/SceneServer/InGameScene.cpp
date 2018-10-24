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
	// �Ƹ� ��Ʈ��ũ �ͼ����� ��� Ŭ���̾�Ʈ���� ���� ���� ���� ������ ����. -> ���� �����ž�
//#ifdef _DEBUG
//	std::cout << "!! �׷����� ���µ�, �� ���Ӿ��� SendNetworkExecption�� ȣ��Ǿ����ϴ�. Ȯ�����ּ���. " << "\n" ;
//#endif
	
	// ������ ���� �� ������ ��, �̹� ��Ʈ��ũ ���� ���ܿ���, �ش� ���ӿ� ���� ����� ó������. (������� 1��, �մ� ��� 1��)
	// ���⼭�� �游 �����ְ�, �ش� Ŭ���̾�Ʈ����, MainUIScene������ �̵��� ���.

	InRoomData.ExitRoom(ptr->roomIndex);
	ptr->roomIndex = -1;

	ptr->dataSize = 4;
}
