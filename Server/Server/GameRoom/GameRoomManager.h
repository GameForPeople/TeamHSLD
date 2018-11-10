#pragma once

#include "../PCH/stdafx.h"
#include "../GameRoom/GameRoom.h"
#include "../GameRoom/Room_List.h"
#include "../UserData/Custom_Map_RedBlackTree.h"


struct SocketInfo;

class GameRoomManager {
	room_list waitRoomCont;
	room_list playRoomCont;

public:
	GameRoomManager() = default;
	~GameRoomManager() = default;

public: //for InGameScene

	// True = Create , False = Join
	GameRoom* RandomMatchingProcess(rbTreeNode<string, UserData>* pInUser, GameRoom* pRetRoom, bool& RetBoolBuuffer);

private:
	GameRoom* _CreateRoom(rbTreeNode<string, UserData>* pInUserNode);

	GameRoom* _JoinRoom(rbTreeNode<string, UserData>* pInUserNode, GameRoom* pRetRoom);
	
public:
	void DestroyRoom(SocketInfo* pClient);

#pragma region [old functions]
public:
	//__inline int GetDataProtocol(const int& InRoomIndex, const bool& InIsHost) const
	//{
	//	return rooms[InRoomIndex].GetDataProtocol(InIsHost);
	//}

	//// atomic  -> �̿��ٰ� �ƴ�                                                  // ���۷��� ���� enum ���޴´�.
	//__inline void SetDataProtocol(const int& InRoomIndex, const bool& InIsHost, const int InNewDataProtocol) //= 0)
	//{
	//	rooms[InRoomIndex].SetDataProtocol(InIsHost, InNewDataProtocol);
	//}

	//void GetRoomGameData(const int& InRoomIndex, const bool& InIsHost, int& retIsHostFirst, int& retPlayerMissionIndex, int& retEnemyMissionIndex, int& retSubMissionIndex)
	//{
	//	rooms[InRoomIndex].GetRoomGameData(InIsHost, retIsHostFirst, retPlayerMissionIndex, retEnemyMissionIndex, retSubMissionIndex);
	//}

	//void SetCharacterIndex(const int& InRoomIndex, const bool& InIsHost, const int& InCharacterIndex)
	//{
	//	rooms[InRoomIndex].SetCharacterIndex(InIsHost, InCharacterIndex);
	//}

	//int GetEnemyCharacterIndex(const int& InRoomIndex, const bool& InIsHost)
	//{
	//	// �̰� ���۷����� ��ȯ�ص� �ǳ�...?
	//	return rooms[InRoomIndex].GetEnemyCharacterIndex(InIsHost);
	//}

	//bool SignOut(const int& InRoomIndex, const bool& InIsHost, int& RetEnemyIndex)
	//{
	//	return rooms[InRoomIndex].SignOut(InIsHost, RetEnemyIndex);
	//}

	////newFunction
	//void SetDataBuffer(const int& InRoomIndex, const bool& InIsHost, const char* InBuffer, const int& InCopySize)
	//{
	//	rooms[InRoomIndex].SetDataBuffer(InIsHost, InBuffer, InCopySize);
	//}

	//void GetDataBuffer(const int& InRoomIndex, const bool& InIsHost, char* RetBuffer, const int& InCopySize)
	//{
	//	rooms[InRoomIndex].GetDataBuffer(InIsHost, RetBuffer, InCopySize);
	//}

	//void GetDataBuffer(const int& InRoomIndex, const bool& InIsHost, char* RetBuffer)
	//{
	//	rooms[InRoomIndex].GetDataBuffer(InIsHost, RetBuffer);
	//}
#pragma endregion
};