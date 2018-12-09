#include "../GameRoom/GameRoom.h"

GameRoom::GameRoom(const shared_ptr<UserData>& InHostUserIter, GameRoom* InLeft, GameRoom* InRight)
	:
	roomState(ROOM_STATE::ROOM_STATE_SOLO), 
	left(InLeft), right(InRight), roomDynamicData()
{
	roomDynamicData = new RoomDynamicData;
	
	roomDynamicData->hostNickname = InHostUserIter->GetNickname();
}

GameRoom::GameRoom(const int& InBuffer)
	:
	roomState(),
	roomDynamicData(nullptr),
	left(nullptr), right(nullptr)
{}

GameRoom::~GameRoom()
{
	//if (left != nullptr || right != nullptr)
	//	std::cout << "아마도 GameRoom ERROR 입니다. \n";
	if (roomDynamicData != nullptr)
		delete roomDynamicData;
}

void GameRoom::JoinRoom(const shared_ptr<UserData>& InGuestUserIter)
{
	// 클라에서 JoinRoom하자마자, 클라자체 바로 로딩 들어가고, 호스트는 모르니까 On시켜줌
	//if (roomState == ROOM_STATE::ROOM_STATE_SOLO) {

	//hostDataProtocol = VOID_GAME_STATE;
	//guestDataProtocol = VOID_GAME_STATE;

	// NOTIFY_GAME_READY으로 하고, 두 유저 모두 Void_Game_State인지 확인. 게임 준비가 되면, VOID_GAME_STATE으로 변경함.
	dataProtocol[0] = NOTIFY_GAME_READY;
	dataProtocol[1] = NOTIFY_GAME_READY;

	roomDynamicData->guestNickname = InGuestUserIter->GetNickname();

	// 방 정보 바꾸는게 제일 마지막에 되어야 함.
	roomState = ROOM_STATE::ROOM_STATE_PLAY;
}
