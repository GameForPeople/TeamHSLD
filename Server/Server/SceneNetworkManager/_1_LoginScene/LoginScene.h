#pragma once

#include "../../PCH/stdafx.h"

#include "../BaseScene.h"

#include "../../GameRoom/GameRoomManager.h"
#include "../../UserData/UserDataManager.h"

struct SocketInfo;

namespace SCENE_NETWORK_MANAGER
{
	class LoginScene : public BaseScene
	{
	private:
		const int PERMIT_LOGIN;
		const int FAIL_LOGIN;
		const int PERMIT_NICKNAME; //104

	public:
		LoginScene() = delete;
		LoginScene(GameRoomManager* pInRoomData, UserDataManager* pInUserData, UDPManager* pInUDPManager);
		virtual ~LoginScene() override = default;

	public:
		virtual void ProcessData(const int& InRecvType, SocketInfo* pClient) override;

	private:
		void _RecvDemandLogin(SocketInfo* pClient);

		void __SendPermitLogin(SocketInfo* pClient, const string& InNickName, const int& RetWinCount, const int& RetLoseCount, const int& RetMoney,
			const int& RetAchievementBit, const int& RetTitleBit, const int& RetCharacterBit, const vector<string>& RetFriendStringCont);
		
		void __SendFailLogin(SocketInfo* pClient, const int& RetFailReason);

	private:
		void _RecvChangeNickName(SocketInfo* pClient);
		void __SendChangeNickName(SocketInfo* pClient);
	};
}