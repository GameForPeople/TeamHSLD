#pragma once

#include "../../PCH/stdafx.h"

#include "../BaseScene.h"

struct SocketInfo;
class GameRoomManager;
class UserDataManager;

namespace SCENE_NETWORK_MANAGER
{
	class LobbyScene : public BaseScene
	{
	private:
		const int PERMIT_MAKE_RANDOM;
		const int PERMIT_JOIN_RANDOM;

		const int PERMIT_GUEST_JOIN;
		const int PERMIT_GUEST_NOT_JOIN;

		const int ANSWER_EXIT_RANDOM;	

		const int ANSWER_FRIEND_JOIN; // for friend;
		const int HOSTCHECK_FRIEND_INVITE; // 친구 기능에서, 방을 아에 나갔을 경우 동일하게 처리함.

		const int CONST_TRUE;
		const int CONST_FALSE;

	public:
		LobbyScene() = delete;
		LobbyScene(GameRoomManager* pInRoomData, UserDataManager* pInUserData, UDPManager* pInUDPManager);
		virtual ~LobbyScene() override = default;

	public:
		virtual void ProcessData(const int InRecvType, SocketInfo* pClient) override;

	private:
		void _DemandRandomMatch(SocketInfo* pClient);
		void _DemandGuestJoin(SocketInfo* pClient);

		void _DemandExitRandom(SocketInfo* pClient);
		void _DemandFriendJoin(SocketInfo* pClient);
	};
}