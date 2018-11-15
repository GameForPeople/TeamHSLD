#pragma once

#include "../../PCH/stdafx.h"

#include "../BaseScene.h"

struct SocketInfo;
class GameRoomManager;
class UserDataManager;

namespace SCENE_NETWORK_MANAGER
{
	class InGameScene : public BaseScene
	{
	public:
		InGameScene();
		virtual ~InGameScene() override = default;

	public:
		virtual void ProcessData(const int& InRecvType, SocketInfo* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData) override;

	private:
		//virtual void ProcessRecv(const int& InRecvType, SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData) override;
		void ProcessRecv(const int& InRecvType, SocketInfo* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);

		//InSendType Called By Value로 전달 요구됨.
		void ProcessSend(const int InSendType, SocketInfo* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);

	private:
		//std::vector<FunctionPointer> functionPointers;
		void(InGameScene::*RecvFunctions[10])(SocketInfo* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
		void(InGameScene::*SendFunctions[10])(SocketInfo* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);

		void RecvGameState(SocketInfo* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
		void RecvChangeTurn(SocketInfo* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
		void RecvDiceValue(SocketInfo* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
		void RecvTerrainType(SocketInfo* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
		void RecvTerrainIndexs(SocketInfo* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
		void RecvEventcardIndex(SocketInfo* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
		void RecvNetworkExecption(SocketInfo* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
		void RecvGameReady(SocketInfo* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);

		void SendGameState(SocketInfo* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
		void SendChangeTurn(SocketInfo* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
		void SendDiceValue(SocketInfo* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
		void SendTerrainType(SocketInfo* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
		void SendTerrainIndexs(SocketInfo* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
		void SendEventcardIndex(SocketInfo* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
		void SendNetworkExecption(SocketInfo* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
		void SendGameReady(SocketInfo* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
	};

	//void RecvGameState(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
	//void RecvChangeTurn(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
	//void RecvDiceValue(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
	//void RecvTerrainType(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
	//void RecvTerrainIndexs(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
	//void RecvEventcardIndex(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
	//void RecvNetworkExecption(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
	//
	//void SendGameState(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
	//void SendChangeTurn(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
	//void SendDiceValue(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
	//void SendTerrainType(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
	//void SendTerrainIndexs(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
	//void SendEventcardIndex(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
	//void SendNetworkExecption(SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData);
}