#include "../GameRoom/NewGameRoom.h"

NewGameRoom::NewGameRoom(user_iter* InHostUserIter, NewGameRoom* InLeft, NewGameRoom* InRight)
	:roomState(ROOM_STATE::ROOM_STATE_SOLO), 
	hostMissionIndex(rand() % 5), guestMissionIndex(rand() % 5), subMissionIndex(rand() % 5), 
	hostCharacterIndex(1), guestCharacterIndex(1), isHostFirst(rand() % 2),
	left(InLeft), right(InRight)
{
	pUserIter[0] = InHostUserIter;
}

NewGameRoom::NewGameRoom(const int& InBuffer)
	:hostMissionIndex(), guestMissionIndex(), subMissionIndex(),
	hostCharacterIndex(), guestCharacterIndex(), isHostFirst(),
	left(nullptr), right(nullptr)
{}

NewGameRoom::~NewGameRoom()
{
	//if (left != nullptr || right != nullptr)
	//	std::cout << "아마도 GameRoom ERROR 입니다. \n";
}

void NewGameRoom::JoinRoom(user_iter* InGuestUserIter)
{
	// 클라에서 JoinRoom하자마자, 클라자체 바로 로딩 들어가고, 호스트는 모르니까 On시켜줌
	//if (roomState == ROOM_STATE::ROOM_STATE_SOLO) {

	//hostDataProtocol = VOID_GAME_STATE;
	//guestDataProtocol = VOID_GAME_STATE;

	roomState = ROOM_STATE::ROOM_STATE_WAIT;

	dataProtocol[0] = VOID_GAME_STATE;
	dataProtocol[1] = VOID_GAME_STATE;

	pUserIter[1] = InGuestUserIter;
}
