#pragma once

#include "../stdafx.h"

#include "../SceneServer/BaseScene.h"

namespace SCENE_NETWORK_MANAGER
{
	class InGameScene : public BaseScene
	{
	public:
		InGameScene();
		virtual ~InGameScene() override = default;

	public:
		void ProcessData(const int& InRecvType, SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);

		//virtual void ProcessRecv(const int& InRecvType, SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData) override;
		void ProcessRecv(const int& InRecvType, SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);

		void ProcessSend(const int& InSendType,	SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);

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

	void SendGameState(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
	void SendChangeTurn(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
	void SendDiceValue(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
	void SendTerrainType(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
	void SendTerrainIndexs(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
	void SendEventcardIndex(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
}