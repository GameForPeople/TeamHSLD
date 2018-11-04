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

	public:
		LoginScene();
		virtual ~LoginScene() override = default;
	public:
		virtual void ProcessData(const int& InRecvType, SocketInfo* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData) override;

	public:
		//void ProcessRecv(const int& InRecvType, SOCKETINFO* ptr, UserDataManager& InUserData);
		//void ProcessSend(const int& InSendType, SOCKETINFO* ptr, UserDataManager& InUserData);

	private:
		void RecvDemandLogin(SocketInfo* ptr, UserDataManager& InUserData);

		void SendPermitLogin(SocketInfo* ptr, const string& InNickName, const int& RetWinCount, const int& RetLoseCount, const int& RetMoney,
			const int& RetAchievementBit, const int& RetTitleBit, const int& RetCharacterBit, const vector<string>& RetFriendStringCont);
		
		void SendFailLogin(SocketInfo* ptr, const int& RetFailReason);

	private:
		//int LoginTest(SocketInfo* ptr, UserDataManager& InUserData, const string& InIdBuffer, const int& InPwBuffer, int& outWinCount, int& outLoseCount, int& outMoney);
		//int SignUpTest(SocketInfo* ptr, UserDataManager& InUserData, const string& InIdBuffer, const int& InPwBuffer);
		int LoginProcess(SocketInfo* pInSocketInfo, UserDataManager& InUserData, const string& InID, string& RetNickName, int& RetWinCount, int& RetLoseCount, int& RetMoney,
			int& RetAchievementBit, int& RetTitleBit, int& RetCharacterBit, vector<string>& RetFriendStringCont);
	};
}