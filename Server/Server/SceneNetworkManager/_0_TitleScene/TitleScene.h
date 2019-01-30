#pragma once

#include "../../PCH/stdafx.h"

#include "../BaseScene.h"

#include "../../GameRoom/GameRoomManager.h"
#include "../../UserData/UserDataManager.h"
#include "../../IOCPServer/UDPManager.h"

//DEV_80
// Title Scene에서, 일부 다이렉트 메세지, CoreUI, UDP에 대한 기능도 관리합니다.

namespace SCENE_NETWORK_MANAGER
{
	class TitleScene : public BaseScene
	{
		const int CONST_ANSWER_ANNOUNCEMENT;

	public:
		TitleScene() = delete;
		TitleScene(GameRoomManager* pInRoomData, UserDataManager* pInUserData, UDPManager* pInUDPManager);
		virtual ~TitleScene() override = default;

	public:
		virtual void ProcessData(const int InRecvType, SocketInfo* pClient) override;

	private:
		void _RecvDemandAnnouncement(SocketInfo* pClient);
	};
}