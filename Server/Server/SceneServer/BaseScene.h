#pragma once

#include "../stdafx.h"

#include "../GameRoom/GameRoomManager.h"
#include "../UserData/UserDataManager.h"

//struct FunctionPointer
//{
//	void* functionPointer;
//};

namespace SCENE_NETWORK_MANAGER
{
	enum SCENE_PROTOCOL_START_INDEX
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
		BaseScene() = default;
		//BaseScene(int InSceneIndex) : sceneProtocolStartIndex(InSceneIndex) {};
		virtual ~BaseScene() = 0 {};

	protected:
		//const int sceneProtocolStartIndex;

	public:
		virtual void ProcessData(const int& InType, SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData) = 0;
		//virtual void ProcessData(const int& InType, SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData) = 0;
		//virtual void ProcessRecv(const int& InRecvType, SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData) = 0;
		//virtual void ProcessSend(const int& InSendType, SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData) = 0;
	};
}