#pragma once

#include "../stdafx.h"

#include "../SceneServer/BaseScene.h"

#include "../GameRoom/GameRoomManager.h"
#include "../UserData/UserDataManager.h"

namespace SCENE_NETWORK_MANAGER
{
	class RoomScene : public BaseScene
	{
	private:
		const int PERMIT_ENEMY_CHARACTER;

	public:
		RoomScene();
		virtual ~RoomScene() override = default;

	public:
		virtual void ProcessData(const int& InRecvType, SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData) override;

	public:
		//void ProcessRecv(const int& InRecvType,	SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
		//void ProcessSend(const int& InSendType,	SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);

	private:
		//void(*RecvFunctions[1])(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
		//void(*SendFunctions[1])(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);

	private:
		void DemandEnemyCharacterIndex(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
	};
}