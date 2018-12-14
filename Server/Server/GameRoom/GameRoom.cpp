#include "../GameRoom/GameRoom.h"

GameRoom::GameRoom(const shared_ptr<UserData>& InHostUserIter, /*GameRoom* InLeft, GameRoom* InRight,*/ bool InIsFriendMode)
	:	roomState(ROOM_STATE::ROOM_STATE_SOLO)
	//,	left(InLeft), right(InRight)
	,	roomDynamicData(make_shared<RoomDynamicData>(InHostUserIter->GetNickname()))
	,	isFriendMode(InIsFriendMode)
	,	dataProtocol()
	,	dataBuffer()
{}

GameRoom::~GameRoom()
{
	roomDynamicData.reset();
}

void GameRoom::JoinRoom(const shared_ptr<UserData>& InGuestUserIter)
{
	// Ŭ�󿡼� JoinRoom���ڸ���, Ŭ����ü �ٷ� �ε� ����, ȣ��Ʈ�� �𸣴ϱ� On������
	//if (roomState == ROOM_STATE::ROOM_STATE_SOLO) {

	//hostDataProtocol = VOID_GAME_STATE;
	//guestDataProtocol = VOID_GAME_STATE;

	// NOTIFY_GAME_READY���� �ϰ�, �� ���� ��� Void_Game_State���� Ȯ��. ���� �غ� �Ǹ�, VOID_GAME_STATE���� ������.
	dataProtocol[0] = NOTIFY_GAME_READY;
	dataProtocol[1] = NOTIFY_GAME_READY;

	roomDynamicData->guestNickname = InGuestUserIter->GetNickname();

	// �� ���� �ٲٴ°� ���� �������� �Ǿ�� ��.
	roomState = ROOM_STATE::ROOM_STATE_PLAY;

	// �߱��ϴ� ����â ������ �ǹ��ֿ��ϱ� ��ȭ�޴� ô
}
