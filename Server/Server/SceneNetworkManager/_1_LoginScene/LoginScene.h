#pragma once

#include "../../PCH/stdafx.h"

#include "../BaseScene.h"

#include "../../GameRoom/GameRoomManager.h"
#include "../../UserData/NewUserDataManager.h"

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
		virtual void ProcessData(const int& InRecvType, SocketInfo* ptr, GameRoomManager& InRoomData, NewUserDataManager& InUserData) override;

	public:
		//void ProcessRecv(const int& InRecvType, SOCKETINFO* ptr, UserDataManager& InUserData);
		//void ProcessSend(const int& InSendType, SOCKETINFO* ptr, UserDataManager& InUserData);

	private:
		void RecvDemandLogin(SocketInfo* ptr, NewUserDataManager& InUserData);

		void SendPermitLogin(SocketInfo* ptr, const int& InWinCount, const int& InLoseCount, const int& InMoney);
		void SendFailLogin(SocketInfo* ptr, const int& RetFailReason);

	private:
		int LoginTest(SocketInfo* ptr, NewUserDataManager& InUserData, const string& InIdBuffer, const int& InPwBuffer, int& outWinCount, int& outLoseCount, int& outMoney);
		int SignUpTest(SocketInfo* ptr, NewUserDataManager& InUserData, const string& InIdBuffer, const int& InPwBuffer);
	};
}