#pragma once

#include "../../PCH/stdafx.h"

#include "../BaseScene.h"

#include "../../GameRoom/GameRoomManager.h"
#include "../../UserData/UserDataManager.h"

struct SocketInfo;

namespace SCENE_NETWORK_MANAGER
{
	class MainUiScene : public BaseScene
	{
		const int PERMIT_FRIEND_INFO;
		const int CONST_TRUE;
		const int CONST_FALSE;

	public:
		MainUiScene();
		virtual ~MainUiScene() override = default;

	public:
		virtual void ProcessData(const int& InRecvType, SocketInfo* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData) override;

	public:
		//void ProcessRecv(const int& InRecvType,	SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
		//void ProcessSend(const int& InSendType,	SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);

	private:
		//void(*RecvFunctions[1])(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
		//void(*SendFunctions[1])(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
	private:
		void _DemandFriendInfoProcess(SocketInfo* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
	};
}