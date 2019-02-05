#pragma once

#include "../PCH/stdafx.h"
#include "../GameRoom/GameRoom.h"
#include "../Custom_DataStructure/Custom_Node_List.h"

struct SocketInfo;

class GameRoomManager {
	// 방 관리 안하도록 변경.
	//CUSTOM_LIST::CustomList<GameRoom, shared_ptr<UserData>> waitRoomCont;
	//CUSTOM_LIST::CustomList<GameRoom, shared_ptr<UserData>> playRoomCont;

	weak_ptr<GameRoom>		pWaitRoom;
	
	atomic<int>				roomNum;
	
	CRITICAL_SECTION		csRoom;

public:
	GameRoomManager() noexcept;
	~GameRoomManager();

public:
	// RandomMatchingProcess, _CreateRoom, _JoinRoom에서 &를 사용해도, 레퍼런스 카운터 1이하로 안떨어질 듯.

	_DEPRECATED bool RandomMatchingProcess(const shared_ptr<UserData>& pInUser); 	// True = Create , False = Join

private:
	void _CreateRoom(const shared_ptr<UserData>& pClient);
	void _JoinRoom(const shared_ptr<UserData>& pClient);
	void _DestroyRoom(SocketInfo* pClient);
	
public:
	bool HostCancelWaiting(SocketInfo* pClient);
	
	_NODISCARD _inline int GetRoomNum() const noexcept { return roomNum; }
public:
	_DEPRECATED void DEBUG_PRINT_WAIT_EMPTY( /*int InValue */) const
	{
		if(pWaitRoom.expired() == true)
			std::cout << "현재 게임 대기중인 유저가 없습니다. \n";
		else
			std::cout << "현재 게임을 위해 대기중인 유저가 있습니다. \n";
	}

#pragma region [old functions]
public:
	//__inline int GetDataProtocol(const int& InRoomIndex, const bool& InIsHost) const
	//{
	//	return rooms[InRoomIndex].GetDataProtocol(InIsHost);
	//}

	//// atomic  -> 이였다가 아님                                                  // 레퍼런스 쓰면 enum 못받는다.
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
	//	// 이거 레퍼런스로 반환해도 되나...?
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