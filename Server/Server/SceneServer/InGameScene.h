#pragma once

#include "../SceneServer/BaseScene.h"

namespace INGAME_SCENE
{
	class InGameScene : public BaseScene
	{
	public:
		InGameScene();
		virtual ~InGameScene() override = default;

	public:
		virtual void ProcessRecv(const int& InRecvType,
			SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData) override;

	private:
		//std::vector<FunctionPointer> functionPointers;
		void(*RecvFunctions[10])(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);

		void(InGameScene::*SendFunctions[10])(const int& InRecvType,
			SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);

	private:
		void SendGameState();
		void SendChangeTurn();
		void SendDiceValue();
		void SendTerrainType();
		void SendTerrainIndexs();
		void SendEventcardIndex();
	};

	void RecvGameState(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
	void RecvChangeTurn(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
	void RecvDiceValue(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
	void RecvTerrainType(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
	void RecvTerrainIndexs(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
	void RecvEventcardIndex(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
}