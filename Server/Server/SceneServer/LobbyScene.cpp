#include "../SceneServer/LobbyScene.h"

SCENE_NETWORK_MANAGER::LobbyScene::LobbyScene() 
	: BaseScene(), PERMIT_MAKE_RANDOM(Protocol::PERMIT_MAKE_RANDOM), PERMIT_JOIN_RANDOM(Protocol::PERMIT_JOIN_RANDOM),
	PERMIT_GUEST_JOIN(Protocol::PERMIT_GUEST_JOIN), PERMIT_GUEST_NOT_JOIN(Protocol::PERMIT_GUEST_NOT_JOIN)
{

}

void SCENE_NETWORK_MANAGER::LobbyScene::ProcessData(const int& InRecvType, SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{
	if (InRecvType == DEMAND_RANDOM_MATCH)
	{
		DemandRandomMatch(ptr, InRoomData, InUserData);
	}
	else if(InRecvType == DEMAND_GUEST_JOIN)
	{
		DemandGuestJoin(ptr, InRoomData, InUserData);
	}
}

void SCENE_NETWORK_MANAGER::LobbyScene::DemandRandomMatch(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{
	if (InRoomData.RandomMatchingProcess(ptr->userIndex, ptr->roomIndex)) // Create!!
	{
		ptr->isHost = true;

		int retIsHostFirst, retPlayerMissionIndex, retEnemyMissionIndex, retSubMissionIndex;
		InRoomData.GetRoomGameData(ptr->roomIndex, ptr->isHost, retIsHostFirst, retPlayerMissionIndex, retEnemyMissionIndex, retSubMissionIndex);

		//ptr->dataBuffer = new PermitMakeRandomStruct(ptr->roomIndex, retIsHostFirst, retPlayerMissionIndex, retEnemyMissionIndex, retSubMissionIndex);
		memcpy(ptr->buf, (char*)&PERMIT_MAKE_RANDOM, sizeof(int));
		memcpy(ptr->buf + 4, (char*)&ptr->roomIndex, sizeof(int));
		memcpy(ptr->buf + 8, (char*)&retIsHostFirst, sizeof(int));
		memcpy(ptr->buf + 12, (char*)&retPlayerMissionIndex, sizeof(int));
		memcpy(ptr->buf + 16, (char*)&retEnemyMissionIndex, sizeof(int));
		memcpy(ptr->buf + 20, (char*)&retSubMissionIndex, sizeof(int));

		ptr->dataSize = 24;
	}
	else // Join!
	{
		ptr->isHost = false;

		int retIsHostFirst, retPlayerMissionIndex, retEnemyMissionIndex, retSubMissionIndex;
		InRoomData.GetRoomGameData(ptr->roomIndex, ptr->isHost, retIsHostFirst, retPlayerMissionIndex, retEnemyMissionIndex, retSubMissionIndex);

		string stringBuffer(InUserData.GetUserID(InRoomData.GetEnemyIndex(ptr->roomIndex, ptr->isHost)));
		int sizeBuffer = stringBuffer.size();

		memcpy(ptr->buf, (char*)&PERMIT_JOIN_RANDOM, sizeof(int));
		memcpy(ptr->buf + 4, (char*)&ptr->roomIndex, sizeof(int));
		memcpy(ptr->buf + 8, (char*)&retIsHostFirst, sizeof(int));
		memcpy(ptr->buf + 12, (char*)&retPlayerMissionIndex, sizeof(int));
		memcpy(ptr->buf + 16, (char*)&retEnemyMissionIndex, sizeof(int));
		memcpy(ptr->buf + 20, (char*)&retSubMissionIndex, sizeof(int));
		memcpy(ptr->buf + 24, (char*)&sizeBuffer, sizeof(int));
		memcpy(ptr->buf + 28, stringBuffer.data(), sizeBuffer);

		ptr->dataSize = 28 + sizeBuffer;
	}
}

void SCENE_NETWORK_MANAGER::LobbyScene::DemandGuestJoin(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{
	if (InRoomData.GetGameReady(ptr->roomIndex))
	{
		string stringBuffer(InUserData.GetUserID(InRoomData.GetEnemyIndex(ptr->roomIndex, ptr->isHost)));
		int sizeBuffer = stringBuffer.size();

		memcpy(ptr->buf, (char*)&PERMIT_GUEST_JOIN, sizeof(int));
		memcpy(ptr->buf + 4, (char*)&sizeBuffer, sizeof(int));
		memcpy(ptr->buf + 8, stringBuffer.data(), sizeof(int));

		ptr->dataSize = 8 + sizeBuffer;

		//if (NETWORK_UTIL::SendProcess(ptr, sizeof(int) + sizeof(PermitGuestJoinStruct), PERMIT_GUEST_JOIN))
		//	continue;
	}
	else
	{
		memcpy(ptr->buf, (char*)&PERMIT_GUEST_NOT_JOIN, sizeof(int));

		ptr->dataSize = 4;

		//if (NETWORK_UTIL::SendProcess(ptr, sizeof(int), PERMIT_GUEST_NOT_JOIN))
		//	continue;
	}
}