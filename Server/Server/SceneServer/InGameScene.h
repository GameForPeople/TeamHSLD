#pragma once

#include "../stdafx.h"

#include "../SceneServer/BaseScene.h"

#include "../GameRoom/GameRoomManager.h"
#include "../UserData/UserDataManager.h"

namespace SCENE_NETWORK_MANAGER
{
	class InGameScene : public BaseScene
	{
	public:
		InGameScene();
		virtual ~InGameScene() override = default;

	public:
		virtual void ProcessData(const int& InRecvType, SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData) override;

		//virtual void ProcessRecv(const int& InRecvType, SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData) override;
		void ProcessRecv(const int& InRecvType, SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);

		//InSendType Called By Value로 전달 요구됨.
		void ProcessSend(const int InSendType,	SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);

	private:
		//std::vector<FunctionPointer> functionPointers;
		void(*RecvFunctions[10])(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
		void(*SendFunctions[10])(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);

	};

	void RecvGameState(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
	void RecvChangeTurn(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
	void RecvDiceValue(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
	void RecvTerrainType(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
	void RecvTerrainIndexs(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
	void RecvEventcardIndex(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
	void RecvNetworkExecption(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);

	void SendGameState(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
	void SendChangeTurn(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
	void SendDiceValue(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
	void SendTerrainType(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
	void SendTerrainIndexs(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
	void SendEventcardIndex(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
	void SendNetworkExecption(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
}