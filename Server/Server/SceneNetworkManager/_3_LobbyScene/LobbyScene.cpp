#include "../_3_LobbyScene/LobbyScene.h"

#include "../../IOCPServer/SocketInfo.h"
#include "../../UserData/UserDataManager.h"
#include "../../GameRoom/GameRoomManager.h"
#include "../../IOCPServer/UDPManager.h"

SCENE_NETWORK_MANAGER::LobbyScene::LobbyScene(GameRoomManager* pInRoomData, UserDataManager* pInUserData, UDPManager* pInUDPManager)
	: BaseScene(pInRoomData, pInUserData, pInUDPManager)
	, PERMIT_MAKE_RANDOM(Protocol::PERMIT_MAKE_RANDOM)
	, PERMIT_JOIN_RANDOM(Protocol::PERMIT_JOIN_RANDOM)
	, PERMIT_GUEST_JOIN(Protocol::PERMIT_GUEST_JOIN)
	, PERMIT_GUEST_NOT_JOIN(Protocol::PERMIT_GUEST_NOT_JOIN)
	, ANSWER_EXIT_RANDOM(Protocol::ANSWER_EXIT_RANDOM)
	, ANSWER_FRIEND_JOIN(Protocol::ANSWER_FRIEND_JOIN)
	, HOSTCHECK_FRIEND_INVITE(Protocol::HOSTCHECK_FRIEND_INVITE)
	, CONST_TRUE(1)
	, CONST_FALSE(0)
{

}

void SCENE_NETWORK_MANAGER::LobbyScene::ProcessData(const int& InRecvType, SocketInfo* pClient)
{
	if (InRecvType == DEMAND_RANDOM_MATCH)
			_DemandRandomMatch(pClient);
	else if(InRecvType == DEMAND_GUEST_JOIN)
			_DemandGuestJoin(pClient);
	else if (InRecvType == DEMAND_EXIT_RANDOM)
			_DemandExitRandom(pClient);
	else if (InRecvType == DEMAND_FRIEND_JOIN)
			_DemandFriendJoin(pClient);
}

void SCENE_NETWORK_MANAGER::LobbyScene::_DemandRandomMatch(SocketInfo* pClient)
{
	// 랜덤매칭에서, userIter까지 다 넣음.
	if (pRoomData->RandomMatchingProcess(pClient->pUserNode))
		// Create Room
	{
		 pClient->isHost = true;

		int retIsHostFirst, retPlayerMissionIndex, retEnemyMissionIndex, retSubMissionIndex;
		 pClient->pRoomIter->GetRoomGameData( pClient->isHost, retIsHostFirst, retPlayerMissionIndex, retEnemyMissionIndex, retSubMissionIndex);

		// pClient->dataBuffer = new PermitMakeRandomStruct( pClient->roomIndex, retIsHostFirst, retPlayerMissionIndex, retEnemyMissionIndex, retSubMissionIndex);
		memcpy( pClient->buf, reinterpret_cast<const char*>(&PERMIT_MAKE_RANDOM), sizeof(int));
		memcpy( pClient->buf + 4, reinterpret_cast<char*>(&retIsHostFirst), sizeof(int));
		memcpy( pClient->buf + 8, reinterpret_cast<char*>(&retPlayerMissionIndex), sizeof(int));
		memcpy( pClient->buf + 12, reinterpret_cast<char*>(&retEnemyMissionIndex), sizeof(int));
		memcpy( pClient->buf + 16, reinterpret_cast<char*>(&retSubMissionIndex), sizeof(int));

		pClient->dataSize = 20;
	}
	else
		// Join Room
	{
		 pClient->isHost = false;

		int retIsHostFirst, retPlayerMissionIndex, retEnemyMissionIndex, retSubMissionIndex;
		Type_Nickname stringBuffer;

		pClient->pRoomIter->GetRoomGameDataWithNickname( pClient->isHost, retIsHostFirst, retPlayerMissionIndex, retEnemyMissionIndex, retSubMissionIndex, stringBuffer);

		int sizeBuffer = stringBuffer.size();

		memcpy( pClient->buf, reinterpret_cast<const char*>(&PERMIT_JOIN_RANDOM), sizeof(int));
		memcpy( pClient->buf + 4, reinterpret_cast<char*>(&retIsHostFirst), sizeof(int));
		memcpy( pClient->buf + 8, reinterpret_cast<char*>(&retPlayerMissionIndex), sizeof(int));
		memcpy( pClient->buf + 12, reinterpret_cast<char*>(&retEnemyMissionIndex), sizeof(int));
		memcpy( pClient->buf + 16, reinterpret_cast<char*>(&retSubMissionIndex), sizeof(int));
		memcpy( pClient->buf + 20, reinterpret_cast<char*>(&sizeBuffer), sizeof(int));
		memcpy( pClient->buf + 24, stringBuffer.data(), sizeBuffer);

		 pClient->dataSize = 28 + sizeBuffer;
	}
}

void SCENE_NETWORK_MANAGER::LobbyScene::_DemandGuestJoin(SocketInfo* pClient)
{
	if ( pClient->pRoomIter->GetGamePlay())
	{
		//rbTreeNode<string,UserData>* EnemyPtrBuffer = pClient->pRoomIter->RetEnemyUserIter( pClient->isHost);
		Type_Nickname stringBuffer(pClient->pRoomIter->GetEnemyNickname(pClient->isHost));
		int sizeBuffer = stringBuffer.size(); //DEV_66

		memcpy( pClient->buf, reinterpret_cast<const char*>(&PERMIT_GUEST_JOIN), sizeof(int));
		memcpy( pClient->buf + 4, reinterpret_cast<char*>(&sizeBuffer), sizeof(int));
		memcpy( pClient->buf + 8, stringBuffer.data(), sizeBuffer);

		pClient->dataSize = 8 + sizeBuffer;
	}
	else
	{
		memcpy( pClient->buf, reinterpret_cast<const char*>(&PERMIT_GUEST_NOT_JOIN), sizeof(int));

		pClient->dataSize = 4;
	}
}

void SCENE_NETWORK_MANAGER::LobbyScene::_DemandExitRandom(SocketInfo* pClient)
{
	if (pRoomData->HostCancelWaiting(pClient))
		// 랜덤 매칭 취소가 성공함.
	{
		memcpy( pClient->buf, reinterpret_cast<const char*>(&ANSWER_EXIT_RANDOM), sizeof(int));
		
		pClient->dataSize = 4;
	}
	else
		// 랜덤 매칭 취소가 실패함 (그 사이 다른 게스트가 접속함)
	{
		_DemandGuestJoin(pClient);
	}
}

void SCENE_NETWORK_MANAGER::LobbyScene::_DemandFriendJoin(SocketInfo* pClient)
{
	// 상대방이 거절한 것이 명확함.
	if ( pClient->pRoomIter->GetDynamicFriendInviteBuffer() == false)
	{
		// 상대방이 초대를 거절함을 알림.
		memcpy( pClient->buf, reinterpret_cast<const char*>(&HOSTCHECK_FRIEND_INVITE), sizeof(int));
		memcpy( pClient->buf + 4, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));	// 클라이언트에서 방을 나가야함.

		pClient->dataSize = 8;

		pClient->pRoomIter.reset();
		//pClient->pRoomIter = nullptr;
	}
	// 상대방이 게임 초대를수락함.
	else if ( pClient->pRoomIter->roomState == ROOM_STATE::ROOM_STATE_PLAY)
	{
		//Join Enemy 
		//닉네임 아는데 또 보내야해? 인덱스에 닉네임 있잖아; 그 전에 이미 초대할 때 보내야 옳음.

		memcpy( pClient->buf, reinterpret_cast<const char*>(&ANSWER_FRIEND_JOIN), sizeof(int));
		memcpy( pClient->buf + 4, reinterpret_cast<const char*>(&CONST_TRUE), sizeof(int));

		int retIsHostFirst, retPlayerMissionIndex, retEnemyMissionIndex, retSubMissionIndex;
		
		pClient->pRoomIter->GetRoomGameData( pClient->isHost, retIsHostFirst, retPlayerMissionIndex, retEnemyMissionIndex, retSubMissionIndex);
		
		memcpy( pClient->buf + 8, reinterpret_cast<char*>(&retIsHostFirst), sizeof(int));
		memcpy( pClient->buf + 12, reinterpret_cast<char*>(&retPlayerMissionIndex), sizeof(int));
		memcpy( pClient->buf + 16, reinterpret_cast<char*>(&retEnemyMissionIndex), sizeof(int));
		memcpy( pClient->buf + 20, reinterpret_cast<char*>(&retSubMissionIndex), sizeof(int));

		pClient->dataSize = 24;
	}
	// 아직 어떠한 상황도 아닌 경우. -> UDP소실 가능성, 아직 수락이나 거절 안누른 상태 (7초 대기중)
	else
	{
		memcpy( pClient->buf, reinterpret_cast<const char*>(&ANSWER_FRIEND_JOIN), sizeof(int));
		memcpy( pClient->buf + 4, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));

		pClient->dataSize = 8;
	}
}