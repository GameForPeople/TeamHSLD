#include "../_3_LobbyScene/LobbyScene.h"

#include "../../IOCPServer/SocketInfo.h"
#include "../../UserData/UserDataManager.h"
#include "../../GameRoom/GameRoomManager.h"
#include "../../IOCPServer/UDPManager.h"

SCENE_NETWORK_MANAGER::LobbyScene::LobbyScene() 
	: BaseScene() 
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

void SCENE_NETWORK_MANAGER::LobbyScene::ProcessData(const int& InRecvType, SocketInfo* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData, UDPManager& InUDPManager)
{
	if (InRecvType == DEMAND_RANDOM_MATCH)
	{
		_DemandRandomMatch(ptr, InRoomData, InUserData);
	}
	else if(InRecvType == DEMAND_GUEST_JOIN)
	{
		_DemandGuestJoin(ptr, InRoomData, InUserData);
	}
	else if (InRecvType == DEMAND_EXIT_RANDOM)
	{
		_DemandExitRandom(ptr, InRoomData, InUserData);
	}
	else if (InRecvType == DEMAND_FRIEND_JOIN)
	{
		_DemandFriendJoin(ptr, InRoomData, InUserData);
	}
}

void SCENE_NETWORK_MANAGER::LobbyScene::_DemandRandomMatch(SocketInfo* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{
	bool retBoolBuffer; 
	ptr->pRoomIter = InRoomData.RandomMatchingProcess(ptr->pUserNode, ptr->pRoomIter, retBoolBuffer);
	
	// 랜덤매칭에서, userIter까지 다 넣음.
	if (retBoolBuffer)
		// Create!!
	{
		ptr->isHost = true;

		int retIsHostFirst, retPlayerMissionIndex, retEnemyMissionIndex, retSubMissionIndex;
		ptr->pRoomIter->GetRoomGameData(ptr->isHost, retIsHostFirst, retPlayerMissionIndex, retEnemyMissionIndex, retSubMissionIndex);

		//ptr->dataBuffer = new PermitMakeRandomStruct(ptr->roomIndex, retIsHostFirst, retPlayerMissionIndex, retEnemyMissionIndex, retSubMissionIndex);
		memcpy(ptr->buf, (char*)&PERMIT_MAKE_RANDOM, sizeof(int));
		memcpy(ptr->buf + 4, (char*)&retIsHostFirst, sizeof(int));
		memcpy(ptr->buf + 8, (char*)&retPlayerMissionIndex, sizeof(int));
		memcpy(ptr->buf + 12, (char*)&retEnemyMissionIndex, sizeof(int));
		memcpy(ptr->buf + 16, (char*)&retSubMissionIndex, sizeof(int));

		ptr->dataSize = 20;
	}
	else
		// Join!
	{
		ptr->isHost = false;

		int retIsHostFirst, retPlayerMissionIndex, retEnemyMissionIndex, retSubMissionIndex;
		ptr->pRoomIter->GetRoomGameData(ptr->isHost, retIsHostFirst, retPlayerMissionIndex, retEnemyMissionIndex, retSubMissionIndex);


		rbTreeNode<string,UserData>* EnemyPtrBuffer = ptr->pRoomIter->RetEnemyUserIter(ptr->isHost);
		
		//string stringBuffer((*EnemyIter)->first);
		string stringBuffer(EnemyPtrBuffer->GetValue().GetNickName());
		int sizeBuffer = stringBuffer.size();

		memcpy(ptr->buf, (char*)&PERMIT_JOIN_RANDOM, sizeof(int));
		memcpy(ptr->buf + 4, (char*)&retIsHostFirst, sizeof(int));
		memcpy(ptr->buf + 8, (char*)&retPlayerMissionIndex, sizeof(int));
		memcpy(ptr->buf + 12, (char*)&retEnemyMissionIndex, sizeof(int));
		memcpy(ptr->buf + 16, (char*)&retSubMissionIndex, sizeof(int));
		memcpy(ptr->buf + 20, (char*)&sizeBuffer, sizeof(int));
		memcpy(ptr->buf + 24, stringBuffer.data(), sizeBuffer);

		ptr->dataSize = 28 + sizeBuffer;
	}
}

void SCENE_NETWORK_MANAGER::LobbyScene::_DemandGuestJoin(SocketInfo* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{
	if (ptr->pRoomIter->GetGameReady())
	{
		rbTreeNode<string,UserData>* EnemyPtrBuffer = ptr->pRoomIter->RetEnemyUserIter(ptr->isHost);

		string stringBuffer(EnemyPtrBuffer->GetValue().GetNickName());
		int sizeBuffer = stringBuffer.size();

		memcpy(ptr->buf, reinterpret_cast<const char*>(&PERMIT_GUEST_JOIN), sizeof(int));
		memcpy(ptr->buf + 4, reinterpret_cast<const char*>(&sizeBuffer), sizeof(int));
		memcpy(ptr->buf + 8, stringBuffer.data(), sizeBuffer);

		ptr->dataSize = 8 + sizeBuffer;
	}
	else
	{
		memcpy(ptr->buf, reinterpret_cast<const char*>(&PERMIT_GUEST_NOT_JOIN), sizeof(int));

		ptr->dataSize = 4;
	}
}

void SCENE_NETWORK_MANAGER::LobbyScene::_DemandExitRandom(SocketInfo* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{
	//InRoomData.DEBUG_PRINT_LIST_EMPTY(0);

	if (InRoomData.CancelWait(ptr))
		// 랜덤 매칭 취소가 성공함.
	{
		//InRoomData.DEBUG_PRINT_LIST_EMPTY(0);

		memcpy(ptr->buf, (char*)&ANSWER_EXIT_RANDOM, sizeof(int));
		ptr->dataSize = 4;
	}
	else
		// 랜덤 매칭 취소가 실패함 (그 사이 다른 게스트가 접속함)
	{
		_DemandGuestJoin(ptr, InRoomData, InUserData);
	}
}

void SCENE_NETWORK_MANAGER::LobbyScene::_DemandFriendJoin(SocketInfo* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{
	if (ptr->pRoomIter == nullptr)
	{
		memcpy(ptr->buf, reinterpret_cast<const char*>(&HOSTCHECK_FRIEND_INVITE), sizeof(int));
		memcpy(ptr->buf + 4, reinterpret_cast<const char*>(&CONST_TRUE), sizeof(int));	// 방 나가짐.

		ptr->dataSize = 8;
	}
	else 
	{
		if (ptr->pRoomIter->RetEnemyUserIter(ptr->isHost) == nullptr)
		{
			memcpy(ptr->buf, (char*)&ANSWER_FRIEND_JOIN, sizeof(int));
			memcpy(ptr->buf + 4, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));

			ptr->dataSize = 8;
		}
		else // Join Enemy
		{
			rbTreeNode<string, UserData>* EnemyPtrBuffer = ptr->pRoomIter->RetEnemyUserIter(ptr->isHost);

			string stringBuffer(EnemyPtrBuffer->GetValue().GetNickName());
			int sizeBuffer = stringBuffer.size();

			memcpy(ptr->buf, (char*)&ANSWER_FRIEND_JOIN, sizeof(int));
			memcpy(ptr->buf + 4, reinterpret_cast<const char*>(&CONST_TRUE), sizeof(int));
			memcpy(ptr->buf + 8, (char*)&sizeBuffer, sizeof(int));
			memcpy(ptr->buf + 12, stringBuffer.data(), sizeof(int));

			ptr->dataSize = 12 + sizeBuffer;
		}
	}
}