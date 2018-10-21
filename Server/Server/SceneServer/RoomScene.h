#pragma once

#include "../stdafx.h"

#include "../SceneServer/BaseScene.h"

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
		void ProcessData(const int& InRecvType, SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);

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