#pragma once

#include "../../PCH/stdafx.h"

#include "../BaseScene.h"

#include "../../GameRoom/GameRoomManager.h"
#include "../../UserData/UserDataManager.h"

namespace SCENE_NETWORK_MANAGER
{
	class TitleScene : public BaseScene
	{
	public:
		TitleScene();
		virtual ~TitleScene() override = default;

	public:
		virtual void ProcessData(const int& InRecvType, SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData) override;

		//virtual void ProcessRecv(const int& InRecvType,
		//	SOCKETINFO& ptr, GameRoomManager& InRoomData, UserDataManager& InUserData) override;
		//
		//virtual void ProcessSend(const int& InSendType,
		//	SOCKETINFO& ptr, GameRoomManager& InRoomData, UserDataManager& InUserData) override;

	private:
		//void(*RecvFunctions[1])(SOCKETINFO& ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
		//void(*SendFunctions[1])(SOCKETINFO& ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
	};
}