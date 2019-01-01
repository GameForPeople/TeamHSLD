#pragma once

#include "../../PCH/stdafx.h"

#include "../BaseScene.h"

#include "../../GameRoom/GameRoomManager.h"
#include "../../UserData/UserDataManager.h"

namespace SCENE_NETWORK_MANAGER
{
	class RoomScene : public BaseScene
	{
	private:
		const int PERMIT_ENEMY_CHARACTER;

	public:
		RoomScene() = delete;
		RoomScene(GameRoomManager* pInRoomData, UserDataManager* pInUserData, UDPManager* pInUDPManager);
		virtual ~RoomScene() override = default;

	public:
		virtual void ProcessData(const int InRecvType, SocketInfo* pClient) override;

	private:
		void _DemandEnemyCharacterIndex(SocketInfo* pClient);
	};
}