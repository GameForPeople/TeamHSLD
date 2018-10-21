#pragma once

#include "../stdafx.h"

#include "../SceneServer/BaseScene.h"

namespace SCENE_NETWORK_MANAGER
{
	class LobbyScene : public BaseScene
	{
	private:
		const int PERMIT_MAKE_RANDOM;
		const int PERMIT_JOIN_RANDOM;

		const int PERMIT_GUEST_JOIN;
		const int PERMIT_GUEST_NOT_JOIN;

	public:
		LobbyScene();
		virtual ~LobbyScene() override = default;

	public:
		void ProcessData(const int& InRecvType, SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);

	public:
		//void ProcessRecv(const int& InRecvType,	SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
		//void ProcessSend(const int& InSendType,	SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);

	private:
		//void(*RecvFunctions[1])(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
		//void(*SendFunctions[1])(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);

	private:
		void DemandRandomMatch(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
		void DemandGuestJoin(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
	};
}