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
	//	std::cout << "�Ƹ��� GameRoom ERROR �Դϴ�. \n";
}

void GameRoom::JoinRoom(rbTreeNode<string, UserData>* InGuestUser)
{
	// Ŭ�󿡼� JoinRoom���ڸ���, Ŭ����ü �ٷ� �ε� ����, ȣ��Ʈ�� �𸣴ϱ� On������
	//if (roomState == ROOM_STATE::ROOM_STATE_SOLO) {

	//hostDataProtocol = VOID_GAME_STATE;
	//guestDataProtocol = VOID_GAME_STATE;

	dataProtocol[0] = VOID_GAME_STATE;
	dataProtocol[1] = VOID_GAME_STATE;

	pUserNode[1] = InGuestUser;

	// �� ���� �ٲٴ°� ���� �������� �Ǿ�� ��.
	roomState = ROOM_STATE::ROOM_STATE_PLAY;
}
