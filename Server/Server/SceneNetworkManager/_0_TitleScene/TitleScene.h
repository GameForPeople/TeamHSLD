#pragma once

#include "../../PCH/stdafx.h"

#include "../BaseScene.h"

#include "../../GameRoom/GameRoomManager.h"
#include "../../UserData/UserDataManager.h"
#include "../../IOCPServer/UDPManager.h"


namespace SCENE_NETWORK_MANAGER
{
	class TitleScene : public BaseScene
	{
	public:
		TitleScene() = delete;
		TitleScene(GameRoomManager* pInRoomData, UserDataManager* pInUserData, UDPManager* pInUDPManager);
		virtual ~TitleScene() override = default;

	public:
		virtual void ProcessData(const int& InRecvType, SocketInfo* pClient) override;

	};
}