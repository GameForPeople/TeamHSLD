#pragma once

#include "../../PCH/stdafx.h"

#include "../BaseScene.h"

#include "../../GameRoom/GameRoomManager.h"
#include "../../UserData/UserDataManager.h"
#include "../../IOCPServer/UDPManager.h"

//DEV_80
// Title Scene����, �Ϻ� ���̷�Ʈ �޼���, CoreUI, UDP�� ���� ��ɵ� �����մϴ�.

namespace SCENE_NETWORK_MANAGER
{
	class TitleScene : public BaseScene
	{
		const int CONST_ANSWER_ANNOUNCEMENT;
		
		const int CHAT_BLOCK_SIZE;
		int nowChatIndex;
		std::vector<std::pair< /*Type_Nickname*/ std::wstring , std::wstring>> chatBlockArr;

	public:
		TitleScene() = delete;
		TitleScene(GameRoomManager* pInRoomData, UserDataManager* pInUserData, UDPManager* pInUDPManager);
		virtual ~TitleScene() override = default;

	public:
		virtual void ProcessData(const int InRecvType, SocketInfo* pClient) override;

	private:
		void _RecvDemandAnnouncement(SocketInfo* pClient);
		void _RecvDemandAddChat(SocketInfo* pClient);
	};
}