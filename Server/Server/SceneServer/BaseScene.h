#pragma once

#include "../stdafx.h"

#include "../IOCPServer/IOCPServer.h"

//struct FunctionPointer
//{
//	void* functionPointer;
//};

enum SCENE_PROTOCOL_START_INDEX
{
	INGAMESCENE = 500
};

class BaseScene
{
public:
	BaseScene(const BaseScene&) = delete;
	BaseScene & operator=(const BaseScene&) = delete;

public:
	BaseScene() = delete;
	BaseScene(int InSceneIndex) : sceneProtocolStartIndex (InSceneIndex){};
	virtual ~BaseScene() = default;

protected:
	const int sceneProtocolStartIndex;

public:
	virtual void ProcessRecv(const int& InRecvType, SOCKETINFO* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData) = 0;
};