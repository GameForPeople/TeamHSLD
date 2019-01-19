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

		const int CHECK_DEMAND_MAKE_FRIEND;
		const int NOTIFY_MAKE_FRIEND_INFO;
		const int CHECK_ANSWER_MAKE_FRIEND;

		const int CONST_ANSWER_BUY_ITEM;

		const int CONST_ANSWER_VIP_CODE;

		const int CONST_ANSWER_BUY_CHARACTER;

		const int CONST_PERMIT_ACTIVE_CHARACTER;

		const int CONST_TRUE;
		const int CONST_FALSE;
		const int CONST_2;
		const int CONST_3;

	public:
		MainUiScene() = delete;
		MainUiScene(GameRoomManager* pInRoomData, UserDataManager* pInUserData, UDPManager* pInUDPManager);
		virtual ~MainUiScene() override = default;

	public:
		virtual void ProcessData(const int InRecvType, SocketInfo* pClient) override;

	private:
		void _DemandFriendInfoProcess(SocketInfo* pClient);
		void _DemandFriendInviteProcess(SocketInfo* pClient);
		void _AnswerFriendInviteProcess(SocketInfo* pClient);
		void _DelayFriendInviteProcess(SocketInfo* pClient);
		void _DemandMakeFriendProcess(SocketInfo* pClient);
		void _DemandMakeFriendInfoProcess(SocketInfo* pClient);
		void _AnswerMakeFriendProcess(SocketInfo* pClient);
		void _BuyItemProcess(SocketInfo* pClient);
		void _VipCodeProcess(SocketInfo* pClient);
		void _BuyCharacterProcess(SocketInfo* pClient);
		void _ChangeActiveCharacter(SocketInfo* pClient);
	};
}