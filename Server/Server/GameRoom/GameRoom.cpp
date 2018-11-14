#include "../GameRoom/GameRoom.h"

GameRoom::GameRoom(rbTreeNode<string, UserData>* InHostUser, GameRoom* InLeft, GameRoom* InRight)
	:
	roomState(ROOM_STATE::ROOM_STATE_SOLO), 
	left(InLeft), right(InRight), roomDynamicData(new RoomDynamicData())
{
	pUserNode[0] = InHostUser;
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
	//	std::cout << "�Ƹ��� GameRoom ERROR �Դϴ�. \n";
}

void GameRoom::JoinRoom(rbTreeNode<string, UserData>* InGuestUser)
{
	// Ŭ�󿡼� JoinRoom���ڸ���, Ŭ����ü �ٷ� �ε� ����, ȣ��Ʈ�� �𸣴ϱ� On������
	//if (roomState == ROOM_STATE::ROOM_STATE_SOLO) {

	//hostDataProtocol = VOID_GAME_STATE;
	//guestDataProtocol = VOID_GAME_STATE;

	// NOTIFY_GAME_READY���� �ϰ�, �� ���� ��� Void_Game_State���� Ȯ��. ���� �غ� �Ǹ�, VOID_GAME_STATE���� ������.
	dataProtocol[0] = NOTIFY_GAME_READY;
	dataProtocol[1] = NOTIFY_GAME_READY;

	pUserNode[1] = InGuestUser;

	// �� ���� �ٲٴ°� ���� �������� �Ǿ�� ��.
	roomState = ROOM_STATE::ROOM_STATE_PLAY;
}
