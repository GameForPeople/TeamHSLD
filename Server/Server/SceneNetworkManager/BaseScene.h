#pragma once

#include "../PCH/stdafx.h"

#include "../GameRoom/GameRoomManager.h"
#include "../UserData/UserDataManager.h"
#include "../IOCPServer/UDPManager.h"

//struct FunctionPointer
//{
//	void* functionPointer;
//};

namespace SCENE_NETWORK_MANAGER
{
	enum SCENE_PROTOCOL_START_INDEX : unsigned int
	{
		TITLE_SCENE = 0,
		LOGIN_SCENE = 100,
		MAINUI_SCENE = 200,
		LOBBY_SCENE = 300,
		ROOM_SCENE = 400,
		INGAME_SCENE = 500
	};

	class BaseScene
	{
	public:
		BaseScene(const BaseScene&) = delete;
		BaseScene & operator=(const BaseScene&) = delete;

	public:
		BaseScene() = delete;
		BaseScene(GameRoomManager* pInRoomData, UserDataManager* pInUserData, UDPManager* pInUDPManager)
			: pRoomData(pInRoomData), pUserData(pInUserData), pUDPManager(pInUDPManager)
		{};

		virtual ~BaseScene() = 0 {};

	public:
		GameRoomManager*	pRoomData;	// 해당 리소스는 SceneNetwork 수준에서 제거하면 안됩니다.
		UserDataManager*	pUserData;	// 해당 리소스는 SceneNetwork 수준에서 제거하면 안됩니다.
		UDPManager*			pUDPManager;		// 해당 리소스는 SceneNetwork 수준에서 제거하면 안됩니다.

		//std::reference_wrapper<UserDataManager> refUserData;

	public:
		virtual void ProcessData(const int InType, SocketInfo* pClient) = 0;
	};
}