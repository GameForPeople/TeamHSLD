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
		const int NOTIFY_FRIEND_INVITE;
		const int GUESTCHECK_FRIEND_INVITE;
		const int HOSTCHECK_FRIEND_INVITE;

		const int CONST_TRUE;
		const int CONST_FALSE;

	public:
		MainUiScene();
		virtual ~MainUiScene() override = default;

	public:
		virtual void ProcessData(const int& InRecvType, SocketInfo* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData, UDPManager& InUDPManager) override;

	public:
		//void ProcessRecv(const int& InRecvType,	SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
		//void ProcessSend(const int& InSendType,	SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);

	private:
		//void(*RecvFunctions[1])(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
		//void(*SendFunctions[1])(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
	private:
		void _DemandFriendInfoProcess(SocketInfo* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
		void _DemandFriendInviteProcess(SocketInfo* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData, UDPManager& InUDPManager);
		void _AnswerFriendInviteProcess(SocketInfo* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
		void _DelayFriendInviteProcess(SocketInfo* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
	};
}