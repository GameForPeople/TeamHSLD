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
		InGameScene() = delete;
		InGameScene(GameRoomManager* pInRoomData, UserDataManager* pInUserData, UDPManager* pInUDPManager);
		virtual ~InGameScene() override = default;

	public:
		virtual void ProcessData(const int InRecvType, SocketInfo* pClient) override;

	private:
		//virtual void ProcessRecv(const int& InRecvType, SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData) override;
		void ProcessRecv(const int InRecvType, SocketInfo* pClient);

		//InSendType Called By Value로 전달 요구됨.
		void ProcessSend(const int InSendType, SocketInfo* pClient);

	private:
		void(InGameScene::*RecvFunctions[10])(SocketInfo* pClient);
		void(InGameScene::*SendFunctions[10])(SocketInfo* pClient);

		//std::vector<std::function<void(SocketInfo*)>>recvFunctions;
		//std::vector<std::function<void(SocketInfo*)>>sendFunctions;

		void RecvGameState(SocketInfo* pClient);
		void RecvChangeTurn(SocketInfo* pClient);
		void RecvDiceValue(SocketInfo* pClient);
		void RecvTerrainType(SocketInfo* pClient);
		void RecvTerrainIndexs(SocketInfo* pClient);
		void RecvEventcardIndex(SocketInfo* pClient);
		void RecvNetworkExecption(SocketInfo* pClient);
		void RecvGameReady(SocketInfo* pClient);

		void SendGameState(SocketInfo* pClient);
		void SendChangeTurn(SocketInfo* pClient);
		void SendDiceValue(SocketInfo* pClient);
		void SendTerrainType(SocketInfo* pClient);
		void SendTerrainIndexs(SocketInfo* pClient);
		void SendEventcardIndex(SocketInfo* pClient);
		void SendNetworkExecption(SocketInfo* pClient);
		void SendGameReady(SocketInfo* pClient);
	};
}