#pragma once

#include "../stdafx.h"

#include "../SceneServer/BaseScene.h"

#include "../GameRoom/GameRoomManager.h"
#include "../UserData/UserDataManager.h"

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
		virtual void ProcessData(const int& InRecvType, SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData) override;

	public:
		//void ProcessRecv(const int& InRecvType, SOCKETINFO* ptr, UserDataManager& InUserData);
		//void ProcessSend(const int& InSendType, SOCKETINFO* ptr, UserDataManager& InUserData);

	private:
		void RecvDemandLogin(SOCKETINFO* ptr, UserDataManager& InUserData);

		void SendPermitLogin(SOCKETINFO* ptr, const int& InWinCount, const int& InLoseCount, const int& InMoney);
		void SendFailLogin(SOCKETINFO* ptr, const int& RetFailReason);

	private:
		int LoginTest(SOCKETINFO* ptr, UserDataManager& InUserData, const string& InIdBuffer, const int& InPwBuffer, int& outWinCount, int& outLoseCount, int& outMoney);
		int SignUpTest(SOCKETINFO* ptr, UserDataManager& InUserData, const string& InIdBuffer, const int& InPwBuffer);
	};
}