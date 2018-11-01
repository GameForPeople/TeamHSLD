#include "../GameRoom/GameRoom.h"

GameRoom::GameRoom(user_iter* InHostUserIter, GameRoom* InLeft, GameRoom* InRight)
	:roomState(ROOM_STATE::ROOM_STATE_SOLO), 
	hostMissionIndex(rand() % 5), guestMissionIndex(rand() % 5), subMissionIndex(rand() % 5), 
	hostCharacterIndex(1), guestCharacterIndex(1), isHostFirst(rand() % 2),
	left(InLeft), right(InRight)
{
	pUserIter[0] = InHostUserIter;
}

GameRoom::GameRoom(const int& InBuffer)
	:hostMissionIndex(), guestMissionIndex(), subMissionIndex(),
	hostCharacterIndex(), guestCharacterIndex(), isHostFirst(),
	left(nullptr), right(nullptr)
{}

GameRoom::~GameRoom()
{
	//if (left != nullptr || right != nullptr)
	//	std::cout << "�Ƹ��� GameRoom ERROR �Դϴ�. \n";
}

void GameRoom::JoinRoom(user_iter* InGuestUserIter)
{
	// Ŭ�󿡼� JoinRoom���ڸ���, Ŭ����ü �ٷ� �ε� ����, ȣ��Ʈ�� �𸣴ϱ� On������
	//if (roomState == ROOM_STATE::ROOM_STATE_SOLO) {

	//hostDataProtocol = VOID_GAME_STATE;
	//guestDataProtocol = VOID_GAME_STATE;

	dataProtocol[0] = VOID_GAME_STATE;
	dataProtocol[1] = VOID_GAME_STATE;

	pUserIter[1] = InGuestUserIter;

	// �� ���� �ٲٴ°� ���� �������� �Ǿ�� ��.
	roomState = ROOM_STATE::ROOM_STATE_PLAY;
}
