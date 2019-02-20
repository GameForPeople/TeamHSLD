#include "../_5_InGameScene/InGameScene.h"

#include "../../IOCPServer/SocketInfo.h"

#include "../../GameRoom/GameRoomManager.h"
#include "../../UserData/UserDataManager.h"
#include "../../IOCPServer/UDPManager.h"

SCENE_NETWORK_MANAGER::InGameScene::InGameScene(GameRoomManager* pInRoomData, UserDataManager* pInUserData, UDPManager* pInUDPManager) 
	: BaseScene(pInRoomData, pInUserData, pInUDPManager)
	, CONST_ANSWER_EMOJI(Protocol::NOTIFY_EMOJI)
	, RecvFunctions()
	, SendFunctions()
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
	RecvFunctions[8] = &InGameScene::RecvEmoji;
	RecvFunctions[9] = &InGameScene::RecvGameEnd;
	RecvFunctions[10] = nullptr; //&InGameScene::RecvGameBuffer; // 호출될일 없음
	RecvFunctions[11] = &InGameScene::RecvEventBuffer;


	SendFunctions[0] = &InGameScene::SendGameState;
	SendFunctions[1] = &InGameScene::SendChangeTurn;
	SendFunctions[2] = &InGameScene::SendDiceValue;
	SendFunctions[3] = &InGameScene::SendTerrainType;
	SendFunctions[4] = &InGameScene::SendTerrainIndexs;
	SendFunctions[5] = &InGameScene::SendEventcardIndex;
	SendFunctions[6] = &InGameScene::SendNetworkExecption;
	SendFunctions[7] = &InGameScene::SendGameReady;
	SendFunctions[8] = nullptr; //&InGameScene::SendEmoji;
	SendFunctions[9] = &InGameScene::SendGameEnd;
	SendFunctions[10] = &InGameScene::SendGameBuffer;
	SendFunctions[11] = &InGameScene::SendEventBuffer;
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
	// 모든 게임에 대한 내 클라이언트의 준비가 끝나면, VOID_GAME_STATE으로 변경함 (최초 NOTIFY_GAME_READY 상태임.)
	pClient->pRoomIter->SetDataProtocol(pClient->isHost, VOID_GAME_STATE);

	//----------------------------------- DEV_59 - 1
	// 하나라도 Wait 상태되면, 굳이 
	pClient->pRoomIter->DeleteDynamicData();
	//-----------------------------------
}

/*
	RecvImoji

		- 이모지를 받았을 경우는, 프로토콜에는 영향을 주지 않고, 
		Room내에 있는, 이모지 버퍼면 변경한다.

		- 따라서 SendImoji는 DataProtocol에 의해 호출될 수 없고,
		Send_Void_GameState일 때, 내 이모지 버퍼가 0이 아니면 직접호출함.
*/
void SCENE_NETWORK_MANAGER::InGameScene::RecvEmoji(SocketInfo* pClient)
{
	pClient->pRoomIter->SetEmoji(pClient->isHost, static_cast<BYTE>(reinterpret_cast<int&>(pClient->buf[4])));
}

/*
	RecvGameEnd

	- 게임이 끝났어! 내가 이겼어!를 받음
*/
void SCENE_NETWORK_MANAGER::InGameScene::RecvGameEnd(SocketInfo* pClient)
{
	pClient->pRoomIter->SetDataProtocol(pClient->isHost, NOTIFY_GAME_END);

	// 내가 받아서 보낼거를 NOTIFY_GAME_BUFFER
	pClient->pRoomIter->SetDataProtocol(!pClient->isHost, NOTIFY_GAME_BUFFER);

	// 이방 이제 끝! 안녕! -> Send Game Buffer에서 해당 로직 처리함.
	// pClient->pRoomIter.reset();

	// 나는 이겼엉! -> 축하해!
	pClient->pUserNode->SetGameResult(true);
}


void SCENE_NETWORK_MANAGER::InGameScene::RecvEventBuffer(SocketInfo* pClient)
{
	pClient->pRoomIter->SetDataBuffer(pClient->isHost, pClient->buf + 4, 92);
	pClient->pRoomIter->SetDataProtocol(pClient->isHost, Protocol::NOTIFY_EVENT_CARD);
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
	// 이거를 최초로 받으면, 두 클라 모두 GameReady 상태가 된 것으로 판단.
	if (auto ifBuf = pClient->pRoomIter->GetEmoji(pClient->isHost)
		;	ifBuf.first == true)
	{
		SendEmoji(pClient, ifBuf.second);
		pClient->dataSize = 8;
	}
	else
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

	pClient->dataSize = 80;
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
	// 아마 네트워크 익셉션을 상대 클라이언트에게 직접 보낼 일은 딤져도 없음. -> 닥쳐 있을거야
//#ifdef _DEBUG
	std::cout << "!! 그럴일이 없는데, 인 게임씬의 SendNetworkExecption가 호출되었습니다. 확인해주세요. " << "\n" ;
//#endif
	pUserData->SetGameResult(pClient->pUserNode, false);

	//pRoomData->DestroyRoom(pClient);
	pClient->pRoomIter.reset();

	pClient->dataSize = 4;

	//*IsSaveOn = true;
}

void SCENE_NETWORK_MANAGER::InGameScene::SendGameReady(SocketInfo* pClient)
{
	// 이거 받으면, 507이니, 상대방이 아직 GameReady 상태가 안됨.
	pClient->dataSize = 4;
}

/*
	SendEmoji

	- SendImoji는 DataProtocol에 의해 호출될 수 없고,
		Send_Void_GameState일 때, 내 이모지 버퍼가 0이 아니면 직접호출함.
*/
void SCENE_NETWORK_MANAGER::InGameScene::SendEmoji(SocketInfo* pClient, const BYTE InByte)
{
	int byteToInBuffer = (static_cast<int>(InByte));

	memcpy(pClient->buf, reinterpret_cast<const char*>(&CONST_ANSWER_EMOJI), sizeof(int));
	memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&byteToInBuffer), sizeof(int));
}

/*
	SendGameEnd

	- 마! 니 졋어 임마! 제가 이겻대!
*/
void SCENE_NETWORK_MANAGER::InGameScene::SendGameEnd(SocketInfo* pClient)
{
	// 이방 이제 끝! 안녕!
	pClient->pRoomIter.reset();

	// 나는 져버렸어 ㅠ
	pClient->pUserNode->SetGameResult(false);	

	// 데이터 사이즈는 4
	pClient->dataSize = 4;
}

/*
SendGameBuffer

- 승자가 GameEnd를 보내고, 바로 RoomIter 를 reset해줄 경우, 바로 다음 sendProcess에서 nullptr에러가 뜨기 때문에,
	이와 같이 GameVBuffer에 걸리게하고, 여기서 방을 삭제해주고 처리함.
*/
void SCENE_NETWORK_MANAGER::InGameScene::SendGameBuffer(SocketInfo* pClient)
{
	// 이방 이제 끝! 안녕!
	pClient->pRoomIter.reset();

	// 데이터 사이즈는 4
	pClient->dataSize = 4;
}


void SCENE_NETWORK_MANAGER::InGameScene::SendEventBuffer(SocketInfo* pClient)
{
	pClient->pRoomIter->GetDataBuffer(pClient->isHost, pClient->buf + 4, 92);
	pClient->pRoomIter->SetDataProtocol(!(pClient->isHost), VOID_GAME_STATE);

	pClient->dataSize = 96;
}