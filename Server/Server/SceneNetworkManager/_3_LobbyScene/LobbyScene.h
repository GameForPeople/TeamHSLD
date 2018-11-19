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
		const int HOSTCHECK_FRIEND_INVITE; // ģ�� ��ɿ���, ���� �ƿ� ������ ��� �����ϰ� ó����.

		const int CONST_TRUE;
		const int CONST_FALSE;

	public:
		LobbyScene();
		virtual ~LobbyScene() override = default;

	public:
		virtual void ProcessData(const int& InRecvType, SocketInfo* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData, UDPManager& InUDPManager) override;

	public:
		//void ProcessRecv(const int& InRecvType,	SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
		//void ProcessSend(const int& InSendType,	SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);

	private:
		//void(*RecvFunctions[1])(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
		//void(*SendFunctions[1])(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);

	private:
		void _DemandRandomMatch(SocketInfo* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
		void _DemandGuestJoin(SocketInfo* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);

		void _DemandExitRandom(SocketInfo* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
		void _DemandFriendJoin(SocketInfo* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
	};
}