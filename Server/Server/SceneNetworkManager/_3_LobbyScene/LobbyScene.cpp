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

void SCENE_NETWORK_MANAGER::LobbyScene::ProcessData(const int InRecvType, SocketInfo* pClient)
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
	// ������Ī����, userIter���� �� ����.
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

		memcpy( pClient->buf, reinterpret_cast<const char*>(&PERMIT_JOIN_RANDOM), sizeof(int));
		memcpy( pClient->buf + 4, reinterpret_cast<char*>(&retIsHostFirst), sizeof(int));
		memcpy( pClient->buf + 8, reinterpret_cast<char*>(&retPlayerMissionIndex), sizeof(int));
		memcpy( pClient->buf + 12, reinterpret_cast<char*>(&retEnemyMissionIndex), sizeof(int));
		memcpy( pClient->buf + 16, reinterpret_cast<char*>(&retSubMissionIndex), sizeof(int));
		
		int stringSizeBuffer = MultiByteToWideChar(CP_ACP, 0, &stringBuffer[0], stringBuffer.size(), NULL, NULL);
		wstring uniStrBuffer(stringSizeBuffer, 0);
		MultiByteToWideChar(CP_ACP, 0, &stringBuffer[0], stringBuffer.size(), &uniStrBuffer[0], stringSizeBuffer);
		
		stringSizeBuffer = uniStrBuffer.size() * 2;

		memcpy( pClient->buf + 20, reinterpret_cast<char*>(&stringSizeBuffer), sizeof(int));
		memcpy( pClient->buf + 24, uniStrBuffer.data(), stringSizeBuffer);

		pClient->dataSize = 28 + stringSizeBuffer;
	}
}

void SCENE_NETWORK_MANAGER::LobbyScene::_DemandGuestJoin(SocketInfo* pClient)
{
	if ( pClient->pRoomIter->GetGamePlay())
	{
		//rbTreeNode<string,UserData>* EnemyPtrBuffer = pClient->pRoomIter->RetEnemyUserIter( pClient->isHost);
		Type_Nickname stringBuffer(pClient->pRoomIter->GetEnemyNickname(pClient->isHost));
		int stringSizeBuffer = MultiByteToWideChar(CP_ACP, 0, &stringBuffer[0], stringBuffer.size(), NULL, NULL);
		wstring uniStrBuffer(stringSizeBuffer, 0);
		MultiByteToWideChar(CP_ACP, 0, &stringBuffer[0], stringBuffer.size(), &uniStrBuffer[0], stringSizeBuffer);
		stringSizeBuffer = uniStrBuffer.size() * 2;

		memcpy( pClient->buf, reinterpret_cast<const char*>(&PERMIT_GUEST_JOIN), sizeof(int));
		
		
		memcpy( pClient->buf + 4, reinterpret_cast<char*>(&stringSizeBuffer), sizeof(int));
		memcpy( pClient->buf + 8, uniStrBuffer.data(), stringSizeBuffer);

		pClient->dataSize = 8 + stringSizeBuffer;
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
		// ���� ��Ī ��Ұ� ������.
	{
		memcpy( pClient->buf, reinterpret_cast<const char*>(&ANSWER_EXIT_RANDOM), sizeof(int));
		
		pClient->dataSize = 4;
	}
	else
		// ���� ��Ī ��Ұ� ������ (�� ���� �ٸ� �Խ�Ʈ�� ������)
	{
		_DemandGuestJoin(pClient);
	}
}


/*
	_DemandFriendJoin

	- ģ���� �ʴ� ��, ��� ���� ȣ��Ʈ�� "ģ�� ���� �߾�?"�� ��, ȣ��Ǵ� �Լ��Դϴ�.
	- �� 7�� ���, 1���� �ʴ뿡 �ִ� 7�� ȣ���.

		#0. ����� ���� 4����
			0. ģ�� �ʴ뿡, ģ���� ����! �� ��!
			1. ģ�� �ʴ뿡, ģ���� �Ⱦ�! �� ��!
			2. ���� �ƹ� ���� ���� �Ѥ�
			3. �� �ܿ�...UDP�ҽ� ���� ��....��....
*/
void SCENE_NETWORK_MANAGER::LobbyScene::_DemandFriendJoin(SocketInfo* pClient)
{
	// ������ ������ ���� ��Ȯ��.
	if ( pClient->pRoomIter->GetDynamicFriendInviteBuffer() == false)
	{
		// ������ �ʴ븦 �������� �˸�.
		memcpy( pClient->buf, reinterpret_cast<const char*>(&HOSTCHECK_FRIEND_INVITE), sizeof(int));
		memcpy( pClient->buf + 4, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));	// Ŭ���̾�Ʈ���� ���� ��������.

		pClient->dataSize = 8;

		pClient->pRoomIter.reset();
		//pClient->pRoomIter = nullptr;
	}
	// ������ ���� �ʴ븦������.
	else if ( pClient->pRoomIter->roomState == ROOM_STATE::ROOM_STATE_PLAY)
	{
		//Join Enemy 
		//�г��� �ƴµ� �� ��������? �ε����� �г��� ���ݾ�; �� ���� �̹� �ʴ��� �� ������ ����.

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
	// ���� ��� ��Ȳ�� �ƴ� ���. -> UDP�ҽ� ���ɼ�, ���� �����̳� ���� �ȴ��� ���� (7�� �����)
	else
	{
		memcpy( pClient->buf, reinterpret_cast<const char*>(&ANSWER_FRIEND_JOIN), sizeof(int));
		memcpy( pClient->buf + 4, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));

		pClient->dataSize = 8;
	}
}