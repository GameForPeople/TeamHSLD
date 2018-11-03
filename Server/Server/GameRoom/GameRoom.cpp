#include "../GameRoom/GameRoom.h"

GameRoom::GameRoom(rbTreeNode<string, UserData>* InHostUser, GameRoom* InLeft, GameRoom* InRight)
	:roomState(ROOM_STATE::ROOM_STATE_SOLO), 
	hostMissionIndex(rand() % 5), guestMissionIndex(rand() % 5), subMissionIndex(rand() % 5), 
	hostCharacterIndex(1), guestCharacterIndex(1), isHostFirst(rand() % 2),
	left(InLeft), right(InRight)
{
	pUserNode[0] = InHostUser;
}

GameRoom::GameRoom(const int& InBuffer)
	:hostMissionIndex(), guestMissionIndex(), subMissionIndex(),
	hostCharacterIndex(), guestCharacterIndex(), isHostFirst(),
	left(nullptr), right(nullptr)
{}

GameRoom::~GameRoom()
{
	//if (left != nullptr || right != nullptr)
	//	std::cout << "아마도 GameRoom ERROR 입니다. \n";
}

void GameRoom::JoinRoom(rbTreeNode<string, UserData>* InGuestUser)
{
	// 클라에서 JoinRoom하자마자, 클라자체 바로 로딩 들어가고, 호스트는 모르니까 On시켜줌
	//if (roomState == ROOM_STATE::ROOM_STATE_SOLO) {

	//hostDataProtocol = VOID_GAME_STATE;
	//guestDataProtocol = VOID_GAME_STATE;

	dataProtocol[0] = VOID_GAME_STATE;
	dataProtocol[1] = VOID_GAME_STATE;

	pUserNode[1] = InGuestUser;

	// 방 정보 바꾸는게 제일 마지막에 되어야 함.
	roomState = ROOM_STATE::ROOM_STATE_PLAY;
}
